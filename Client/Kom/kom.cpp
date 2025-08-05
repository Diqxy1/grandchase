
#include "stdafx.h"
#include "kom.h"
#include "blowfish.h"
#include "sha1.h"
#include <sys/stat.h>
#include <ShellAPI.h>
#include "io.h"
#include "XorCRC.h"
#include "..\MassFileLib\CRC32\CRC_32.h"

bool IsFileExists(std::string filename)
{
	if (_access(filename.c_str(), 0) != -1)
		return true;
	return false;
}

void ResetReadOnly(std::string path)
{
	if (IsFileExists(path))
	{
		_chmod(path.c_str(), _S_IWRITE);
	}
}

bool DeleteFileForce(std::string filename)
{
	if (DeleteFileA(filename.c_str()) == 0)
	{
		if (GetLastError() == ERROR_ACCESS_DENIED)
		{
			ResetReadOnly(filename);
			if (DeleteFileA(filename.c_str()) != 0)
				return true;
		}
		else if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			return true;
		}
	}
	else
	{
		return true;
	}
	return false;
}

bool DeleteDirForce(std::string dirname)
{
	char szDir[MAX_PATH + 1] = { 0, };
	SHFILEOPSTRUCTA fos;

	sprintf_s(szDir, "%s%c", dirname.c_str(), '\0');

	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.wFunc = FO_DELETE;
	fos.pFrom = szDir;
	fos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
	return SHFileOperationA(&fos) == 0;
}

KSubfile::KSubfile(Komfile* parent, std::ifstream& stream, int headersize)
	:parentkom(parent), algorithm(infilate), adler32(0), size(0), compressedsize(0), filetime(0), offset(0), iscalcadler(false)
{
	stream.read((char*)& size, 4);
	stream.read((char*)& compressedsize, 4);
	stream.read((char*)& offset, 4);
	offset += headersize;
	filetime = 0;
}

KSubfile::KSubfile(Komfile* parent, xmlNode* node, int* offset)
	:parentkom(parent), algorithm(infilate), adler32(0), size(0), compressedsize(0), filetime(0), offset(0), iscalcadler(true)
{
	xmlAttr* attribute = node->properties;
	while (attribute)
	{
		if (attribute->children && attribute->children->content)
		{
			const int contentlen = strlen((char*)attribute->children->content);

			if (contentlen > 0)
			{
				if (strcmp((char*)attribute->name, "Checksum") == 0)
				{
					_snscanf_s((char*)attribute->children->content, contentlen, "%x", &adler32);
				}
				else if (strcmp((char*)attribute->name, "CompressedSize") == 0)
				{
					_snscanf_s((char*)attribute->children->content, contentlen, "%d", &compressedsize);
				}
				else if (strcmp((char*)attribute->name, "Size") == 0)
				{
					_snscanf_s((char*)attribute->children->content, contentlen, "%d", &size);
				}
				else if (strcmp((char*)attribute->name, "Algorithm") == 0)
				{
					_snscanf_s((char*)attribute->children->content, contentlen, "%d", &algorithm);
				}
				else if (strcmp((char*)attribute->name, "FileTime") == 0)
				{
					_snscanf_s((char*)attribute->children->content, contentlen, "%x", &filetime);
				}
			}
		}
		attribute = attribute->next;
	}

	this->offset = (*offset);
	(*offset) += compressedsize;
}

bool KSubfile::WriteCompressed(std::ostream& stream)
{
	if (parentkom->GetSession())
		return parentkom->GetSession()->WriteRemoteData(parentkom->GetFilename(), stream, offset, compressedsize);

	std::ifstream filestream(parentkom->GetFilename().c_str(), std::ios_base::in | std::ios_base::binary);

	filestream.seekg(offset, std::ios_base::beg);

	int totallen = compressedsize;
	if (totallen < 0)
	{
		filestream.close();
		return false;
	}

	unsigned char buffer[2048];
	int len = 2048;
	while (totallen > 0)
	{
		if (totallen < 2048)
		{
			len = totallen;
		}
		else
		{
			len = 2048;
		}

		filestream.read((char*)buffer, len);
		stream.write((char*)buffer, len);
		if (stream.fail())
		{
			stream.clear();
			return false;
		}
		totallen -= len;
	}
	filestream.close();
	return true;
}

bool KSubfile::WriteCompressed(std::ostream& stream, NETPROGRESS_CALLBACK progress, KCSLockType<SProgressInfo>& progressinfo)
{
	int error;
	if (parentkom->GetSession())
		return parentkom->GetSession()->WriteRemoteData(parentkom->GetFilename(), stream, offset, compressedsize,
			progress, progressinfo, error);

	std::ifstream filestream(parentkom->GetFilename().c_str(), std::ios_base::in | std::ios_base::binary);

	filestream.seekg(offset, std::ios_base::beg);

	int totallen = compressedsize;
	if (totallen < 0)
	{
		filestream.close();
		return false;
	}

	unsigned char buffer[2048];
	int len = 2048;
	while (totallen > 0)
	{
		if (totallen < 2048)
		{
			len = totallen;
		}
		else
		{
			len = 2048;
		}

		filestream.read((char*)buffer, len);
		stream.write((char*)buffer, len);
		if (stream.fail())
		{
			stream.clear();
			return false;
		}
		totallen -= len;
	}
	filestream.close();
	return true;
}

bool KSubfile::WriteCompressed(char* data)
{
	if (parentkom->GetSession())
		return false;

	std::ifstream filestream(parentkom->GetFilename().c_str(), std::ios_base::in | std::ios_base::binary);

	filestream.seekg(offset, std::ios_base::beg);
	filestream.read(data, compressedsize);
	return true;
}

bool KSubfile::WriteDecompressedFile(const std::string fileName)
{
	std::ofstream file(fileName.c_str(), std::ios::binary | std::ios_base::out);

	boost::scoped_array<char> decompressedbuffer(new char[size]);
	if (WriteDecompressed(decompressedbuffer.get()) == false)
		return false;

	file.write(decompressedbuffer.get(), size);
	file.close();

	return true;
}

bool KSubfile::WriteDecompressed(std::ostream& stream)
{
	if (parentkom->GetSession())
		return false;

	boost::scoped_array<char> decompressedbuffer(new char[size]);
	if (WriteDecompressed(decompressedbuffer.get()) == false)
		return false;

	stream.write(decompressedbuffer.get(), size);
	return true;
}

bool KSubfile::WriteDecompressed(char* data)
{
	if (parentkom->GetSession())
		return false;

	boost::scoped_array<char> compressedbuffer(new char[compressedsize]);
	WriteCompressed(compressedbuffer.get());
	unsigned long  ulSize = size;
	DecryptBufferPlus((int)0xC7, compressedbuffer.get(), compressedsize);
	uncompress((BYTE*)(data), &ulSize, (BYTE*)compressedbuffer.get(), compressedsize);
	if (ulSize != size)
		return false;
	return true;
}

bool KSubfile::GetFileData(std::string& _filename, unsigned int& _offset, unsigned int& _compressedsize, unsigned int& _size)
{
	if (parentkom->GetSession())
		return false;
	_filename = parentkom->GetFilename();
	_offset = offset;
	_compressedsize = compressedsize;
	_size = size;
	return true;
}

bool ReadInFile(std::ifstream& filestream, char* buf, int size)
{
	int pos = filestream.tellg();
	filestream.read(buf, size);
	if ((int)filestream.tellg() - pos == size)
		return true;
	filestream.close();
	return false;
}

bool Komfile::ReadInBuffer(char* data, int size)
{
	int sizeq = sizeof(data);
	if (bufPos + size > bufSize)
		return false;
	memcpy((void*)data, &buf[bufPos], size);
	bufPos += size;
	return true;
}

bool Komfile::ReadHeader(int offset, char* data, int length)
{
	XMLDocument doc;
	doc.ReadMemory(data, length);
	if (doc.IsOpen() == false)
		return false;

	xmlNode* files = doc.GetRootElement();
	if (!files)
	{
		return false;
	}

	if (files->type == XML_ELEMENT_NODE && strcmp((char*)files->name, "Files") == 0)
	{
		xmlNode* file = files->children;
		while (file)
		{
			if (file->type == XML_ELEMENT_NODE && strcmp((char*)file->name, "File") == 0)
			{
				std::string key;
				xmlAttr* attribute = file->properties;
				while (attribute)
				{
					if (strcmp((char*)attribute->name, "Name") == 0 && attribute->children)
					{
						key = (char*)attribute->children->content;
						break;
					}
					attribute = attribute->next;
				}
				std::transform(key.begin(), key.end(), key.begin(), tolower);
				subfiles.insert(std::map<std::string, KSubfile>::value_type(key, KSubfile(this, file, &offset)));
			}
			file = file->next;
		}
	}
	return true;
}

bool Komfile::Open(std::string filename)
{
	std::ifstream filestream(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	bool bRet = this->Open(filename, filestream);
	if (bRet == true)
		filestream.close();
	return bRet;
}

bool Komfile::Open(std::string filename, std::ifstream& filestream)
{
	this->filename = filename;

	headerstream.clear();
	Close();
	if (GetFileAttributesA(filename.c_str()) == INVALID_FILE_ATTRIBUTES)
		return false;

	if (filestream.is_open())
	{
		bool isOGN = filename.find(".first") != std::string::npos;

		int buffersize = 0;
		filestream.read((char*)&buffersize, 4);

		buf = new char[buffersize];
		filestream.read(buf, buffersize);

		unsigned int key = 0;
		filestream.read((char*)&key, 4);
		filestream.close();

		DecryptBufferPlus(key, buf, buffersize);

		key = 0;

		WORD version;
		bufPos = 0;
		bufSize = buffersize;
		unsigned int compressed, size;

		if (ReadInBuffer((char*)&filetime, 4) == false) return false;
		if (ReadInBuffer((char*)&size, 4) == false) return false;
		if (ReadInBuffer((char*)&version, 2) == false) return false;
		if (ReadInBuffer((char*)&compressed, 4) == false) return false;
		if (ReadInBuffer((char*)&adler32, 4) == false) return false;
		if (ReadInBuffer((char*)&headersize, 4) == false) return false;

		ULONG	ulFileTime;
		unsigned int nFiles;
		WORD	wVersion;
		UINT	bCompressed;
		UINT	uAdler32;
		UINT	nHeaderXML;

		boost::scoped_array<char> header(new char[headersize]);

		if (ReadInBuffer(header.get(), headersize) == false)
			return false;

		if (isOGN)
			buffersize += 4;

		if (ReadHeader(buffersize + 4, header.get(), headersize) == false)
		{
			headerstream.clear();
			return false;
		}
		return true;
	}
	return false;
}

bool Komfile::Open(std::stringstream& stream, boost::shared_ptr<KomSession> con, std::string url)
{
	this->filename = url;
	this->session = con;

	headerstream.clear();
	Close();

	unsigned int buffersize;
	stream.read((char*)&buffersize, 4);

	char* buffer = new char[buffersize];
	stream.read(buffer, buffersize);

	unsigned int key = 0;
	stream.read((char*)&key, 4);

	DecryptBufferPlus(key, buf, buffersize);

	key = 0;

	int it = 0;
	WORD version;
	unsigned int size, compressed;

	memcpy((char*)&filetime, &buffer[it], 4); it += 4;
	memcpy((char*)&size, &buffer[it], 4); it += 4;
	memcpy((char*)&version, &buffer[it], 2); it += 2;
	memcpy((char*)&compressed, &buffer[it], 4); it += 4;
	memcpy((char*)&adler32, &buffer[it], 4); it += 4;
	memcpy((char*)&headersize, &buffer[it], 4); it += 4;

	XMLDocument doc;
	boost::scoped_array<char> header(new char[headersize]);

	memcpy(header.get(), &buffer[it], headersize);

	headerstream.write((char*)&filetime, 4);
	headerstream.write((char*)&size, 4);
	headerstream.write((char*)&version, 2);
	headerstream.write((char*)&compressed, 4);
	headerstream.write((char*)&adler32, 4);
	headerstream.write((char*)&headersize, 4);

	headerstream.write(header.get(), headersize);

	if (ReadHeader(buffersize + 4, header.get(), headersize) == false)
	{
		headerstream.clear();
		return false;
	}
	return true;
}

bool Komfile::Open(boost::shared_ptr<KomSession> session, std::string url)
{
	this->filename = url;
	this->session = session;

	headerstream.clear();
	Close();

	std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);

	if (session->WriteRemoteData(url, stream, 0, 4) == false)
	{
		return false;
	}

	unsigned int buffersize;
	stream.read((char*)&buffersize, 4);
	stream.clear();

	if (session->WriteRemoteData(url, stream, 4, buffersize) == false)
	{
		return false;
	}

	char* buffer = new char[buffersize];
	stream.read(buffer, buffersize);

	unsigned int key = 0;
	stream.read((char*)&key, 4);

	DecryptBufferPlus(key, buf, buffersize);

	key = 0;

	int it = 0;
	WORD version;
	unsigned int size, compressed;

	memcpy((char*)&filetime, &buffer[it], 4); it += 4;
	memcpy((char*)&size, &buffer[it], 4); it += 4;
	memcpy((char*)&version, &buffer[it], 2); it += 2;
	memcpy((char*)&compressed, &buffer[it], 4); it += 4;
	memcpy((char*)&adler32, &buffer[it], 4); it += 4;
	memcpy((char*)&headersize, &buffer[it], 4); it += 4;

	memcpy((char*)&adler32, &buffer[it], 4); it += 4;
	memcpy((char*)&size, &buffer[it], 4); it += 4;
	memcpy((char*)&filetime, &buffer[it], 4); it += 4;
	memcpy((char*)&compressed, &buffer[it], 4); it += 4;
	memcpy((char*)&headersize, &buffer[it], 4); it += 4;

	XMLDocument doc;
	boost::scoped_array<char> header(new char[headersize]);

	memcpy(header.get(), &buffer[it], headersize);

	headerstream.write((char*)&filetime, 4);
	headerstream.write((char*)&size, 4);
	headerstream.write((char*)&version, 2);
	headerstream.write((char*)&compressed, 4);
	headerstream.write((char*)&adler32, 4);
	headerstream.write((char*)&headersize, 4);

	headerstream.write(header.get(), headersize);

	if (ReadHeader(buffersize + 4, header.get(), headersize) == false)
	{
		headerstream.clear();
		return false;
	}

	return true;
}

bool Komfile::Save(std::string filename, SKomWriteInfo& writeinfo)
{
	std::string tmpfilename = filename + ".tmp";
	std::ofstream file(tmpfilename.c_str(), std::ios::binary);

	if (file.is_open() == false)
	{
		ResetReadOnly(filename);
		int end = filename.find_last_of("\\");
		if (end > 0)
		{
			ResetReadOnly(filename.substr(0, end));
		}
		file.open(filename.c_str(), std::ios_base::binary | std::ios_base::out);
	}

	if (file.is_open() == false)
	{
		return false;
	}

	int headersize = headerstream.tellp();
	std::map<std::string, KSubfile>::iterator i;

	if (headersize > 0)
	{
		boost::scoped_array<char> header(new char[headersize]);
		headerstream.read(header.get(), headersize);
		file.write(header.get(), headersize);
	}
	else
	{
		std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
		char magicword[52] = "KOG GC TEAM MASSFILE V.0.3.";
		unsigned int filetime = GetFileTime();

		stream << "<?xml version=\"1.0\"?><Files>";
		i = subfiles.begin();

		while (i != subfiles.end())
		{
			char tmp[1024];

			_snprintf_s(tmp, 1024, "<File Name=\"%s\" Size=\"%d\" CompressedSize=\"%d\" Checksum=\"%08x\" FileTime=\"%08x\" Algorithm=\"%d\" />",
				i->first.c_str(), i->second.GetSize(), i->second.GetCompressedSize(), i->second.GetAdler32(), i->second.GetFileTime(), i->second.GetAlgorithm());
			stream << tmp;
			i++;
		}
		stream << "</Files>";

		int len = stream.tellp();

		adler32 = AdlerCheckSum::adler32(stream, 0, len, true);

		unsigned int size = subfiles.size();
		unsigned int compress = 1;

		file.write(magicword, 52);
		file.write((char*)& size, 4);
		file.write((char*)& compress, 4);
		file.write((char*)& filetime, 4);
		file.write((char*)& adler32, 4);
		file.write((char*)& len, 4);

		file << stream.str();

		headersize = 72 + len;
	}

	if (writeinfo.IsStop())
	{
		file.close();
		return true;
	}
	writeinfo.Read(headersize);

	i = subfiles.begin();

	KSubfile presubfile;

	while (i != subfiles.end())
	{
		if (i->second.GetParent() == presubfile.GetParent()
			&& i->second.GetOffset() == presubfile.GetOffset() + presubfile.GetCompressedSize())
		{
			presubfile.AddCompressedSize(i->second.GetCompressedSize());
		}
		else
		{
			if (presubfile.GetCompressedSize() > 0)
			{
				SProgressInfo sProgressInfo;
				KCSLOCK_DECLARE(SProgressInfo, progressinfo);
				KCSLOCK_BLOCK(progressinfo)
				{
					writeinfo.GetProgressInfo(progressinfo);
				}

				NETPROGRESS_CALLBACK progress = writeinfo.progress2;

				if (presubfile.WriteCompressed(file, progress, progressinfo) == false)
				{
					file.close();
					return false;
				}

				if (writeinfo.IsStop())
				{
					file.close();
					return true;
				}

				writeinfo.Read(presubfile.GetCompressedSize());
			}

			presubfile = i->second;
		}

		i++;
	}

	if (presubfile.GetCompressedSize() > 0)
	{
		SProgressInfo sProgressInfo;
		KCSLOCK_DECLARE(SProgressInfo, progressinfo);
		KCSLOCK_BLOCK(progressinfo)
		{
			writeinfo.GetProgressInfo(progressinfo);
		}

		NETPROGRESS_CALLBACK progress = writeinfo.progress2;

		if (presubfile.WriteCompressed(file, progress, progressinfo) == false)
		{
			file.close();
			return false;
		}

		if (writeinfo.IsStop())
		{
			file.close();
			return true;
		}
		writeinfo.Read(presubfile.GetCompressedSize());
	}

	file.close();

	if (Komfile::Verify(tmpfilename) == false)
	{
		DeleteFileForce(tmpfilename);
		return false;
	}

	if (GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES)
	{
		if (DeleteFileForce(filename) == false)
		{
			return false;
		}
	}

	if (MoveFileA(tmpfilename.c_str(), filename.c_str()))
	{
		return true;
	}
	return false;
}

void Komfile::Close()
{
	subfiles.clear();

	filetime = 0;
	adler32 = 0;
	headersize = 0;
}

unsigned int Komfile::GetTotalSize()
{
	unsigned int totalsize = 0;
	std::map<std::string, KSubfile>::iterator i = subfiles.begin();
	while (i != subfiles.end())
	{
		totalsize += i->second.GetSize();
		i++;
	}
	return totalsize;
}

unsigned int Komfile::GetFileTime()
{
	unsigned int filetime = 0;
	std::map<std::string, KSubfile>::iterator i = subfiles.begin();
	while (i != subfiles.end())
	{
		filetime += i->second.GetFileTime();
		i++;
	}
	return filetime;
}

bool Komfile::Verify(std::string filename)
{
	Komfile kom;
	if (kom.Open(filename) == false)
		return false;
	return kom.Verify();
}

bool Komfile::Verify()
{
	if (subfiles.empty())
		return true;
	for (const auto& pair : subfiles)
	{
		KSubfile subfile = pair.second;
		std::ifstream file(subfile.GetParent()->GetFilename().c_str(), std::ios::in | std::ios::binary);
		if (!file.is_open())
			return false;
		if (AdlerCheckSum::adler32(file, subfile.GetOffset(), subfile.GetCompressedSize()) != subfile.GetAdler32())
		{
			file.close();
			return false;
		}
		file.close();
	}
	return true;
}

unsigned int KSubfile::GetAdler32()
{
	if (iscalcadler == false)
	{
		std::ifstream file(parentkom->GetFilename().c_str(), std::ios::in | std::ios::binary);
		adler32 = AdlerCheckSum::adler32(file, offset, compressedsize);
		file.close();
		iscalcadler = true;
	}
	return adler32;
}

int Komfile::SeperateMagicWord(char* data, int length)
{
	char* dest = strstr(data, "KOG GC TEAM MASSFILE V.");
	if (dest == NULL)
		return -1;
	int a = 0;
	int b = 0;
	sscanf_s(data, "KOG GC TEAM MASSFILE V.%d.%d", &a, &b);
	return a * 10 + b;
}

bool Komfile::MannualJoin(Komfile& head, Komfile& local, Komfile& remote)
{
	for (auto& entry : head.subfiles)
	{
		const std::string& key = entry.first;
		std::string lowerKey = key;
		std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), tolower);
		auto iter_local = local.subfiles.find(key);
		if (iter_local == local.subfiles.end())
			iter_local = local.subfiles.find(lowerKey);
		auto iter_remote = remote.subfiles.find(key);
		if (iter_remote != remote.subfiles.end())
		{
			KSubfile subfile(iter_remote->second);
			subfiles.emplace(key, subfile);
		}
		else if (iter_local != local.subfiles.end())
		{
			iter_local->second.GetAdler32();
			if (entry.second == iter_local->second)
			{
				KSubfile subfile(iter_local->second);
				subfiles.emplace(key, subfile);
			}
			else
				return false;
		}
		else
			return false;
	}
	headerstream.clear();
	int headersize = head.headerstream.tellp();
	if (headersize > 0)
	{
		std::unique_ptr<char[]> header(new char[headersize]);
		head.headerstream.read(header.get(), headersize);
		headerstream.write(header.get(), headersize);
	}
	return true;
}

int Komfile::LeftOuterJoin(Komfile& remote, Komfile& local)
{
	int re = 0;
	for (auto& pair : remote.subfiles)
	{
		const std::string& key = pair.first;
		std::string lowercaseKey = key;
		std::transform(lowercaseKey.begin(), lowercaseKey.end(), lowercaseKey.begin(), tolower);
		auto iter_local = local.subfiles.find(lowercaseKey);
		if (iter_local == local.subfiles.end())
			iter_local = local.subfiles.find(key);
		if (iter_local != local.subfiles.end())
		{
			iter_local->second.GetAdler32();
			if (pair.second == iter_local->second)
			{
				subfiles.insert(pair);
			}
			else
			{
				subfiles.insert({ pair.first, pair.second });
				re++;
			}
		}
		else
		{
			re++;
			subfiles.insert(pair);
		}
	}
	headerstream.clear();
	int headersize = remote.headerstream.tellp();
	if (headersize > 0)
	{
		std::unique_ptr<char[]> header(new char[headersize]);
		remote.headerstream.read(header.get(), headersize);
		headerstream.write(header.get(), headersize);
	}
	return re;
}

KSubfile Komfile::GetSubfile(std::string filename)
{
	std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
	std::map<std::string, KSubfile>::iterator i = subfiles.find(filename);
	if (i == subfiles.end())
	{
		KSubfile subfile;
		return subfile;
	}
	return i->second;
}

bool Komfile::IsExist(std::string filename)
{
	std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
	std::map<std::string, KSubfile>::iterator i = subfiles.find(filename);
	if (i == subfiles.end())
		return false;
	return true;
}