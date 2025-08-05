#include "stdafx.h"
#include "kom2.h"
#include "blowfish.h"
#include "sha1.h"
#include <sys/stat.h>
#include <ShellAPI.h>
#include "io.h"

std::map<unsigned int, INT64> Komfile::keymap;

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

bool KSubfile::WriteDecompressedFileOnFolder(const std::string fileName, const std::string folderName)
{
	std::ofstream file(".\\" + folderName + "\\" + fileName.c_str(), std::ios::binary | std::ios_base::out);

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
	{
		return true;
	}

	filestream.close();
	return false;
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
	{
		filestream.close();
	}
	return bRet;
}

void DecryptAESHeader(char* data, int length)
{
	for (int i = 0; i < length; i++) {
		data[i] ^= 0x47;
		data[i] ^= 0x72;
		data[i] ^= 0x61;
		data[i] ^= 0x6E;
		data[i] ^= 0x64;
		data[i] ^= 0x20;
		data[i] ^= 0x43;
		data[i] ^= 0x68;
		data[i] ^= 0x61;
		data[i] ^= 0x73;
		data[i] ^= 0x65;
		data[i] ^= 0x20;
		data[i] ^= 0x47;
		data[i] ^= 0x4F;
	}
}
void DecryptV6Header(char* data, int length)
{
	unsigned char key[241] = {
        0x33, 0x39, 0x34, 0x38, 0x35, 0x37, 0x33, 0x63, 0x31, 0x23, 0x52, 0x21,
        0x33, 0x66, 0x31, 0x23, 0x21, 0x23, 0x46, 0x21, 0x23, 0x66, 0x57, 0x43,
        0x77, 0x66, 0x48, 0x25, 0x40, 0x26, 0x68, 0x34, 0x38, 0x33, 0x37, 0x32,
        0x38, 0x18, 0x29, 0x26, 0x4d, 0x25, 0x33, 0x34, 0x42, 0x40, 0x45, 0x62,
        0x32, 0x24, 0x47, 0x32, 0x32, 0x33, 0x39, 0x38, 0x37, 0x34, 0x30, 0x32,
        0x37, 0x30, 0x38, 0x37, 0x39, 0x38, 0x33, 0x40, 0x23, 0x24, 0x47, 0x40,
        0x24, 0x48, 0x38, 0x42, 0x28, 0x20, 0x26, 0x43, 0x41, 0x44, 0x53, 0x43,
        0x56, 0x71, 0x23, 0x24, 0x79, 0x77, 0x24, 0x25, 0x42, 0x65, 0x77, 0x66,
        0x67, 0x33, 0x34, 0x77, 0x71, 0x65, 0x67, 0x68, 0x77, 0x23, 0x24, 0x68,
        0x73, 0x64, 0x67, 0x66, 0x21, 0x66, 0x72, 0x31, 0x38, 0x32, 0x33, 0x35,
        0x39, 0x38, 0x62, 0x30, 0x26, 0x61, 0x42, 0x33, 0x24, 0x62, 0x39, 0x32,
        0x33, 0x36, 0x34, 0x31, 0x34, 0x63, 0x40, 0x25, 0x15, 0x21, 0x40, 0x23,
        0x54, 0x46, 0x63, 0x31, 0x23, 0x24, 0x31, 0x64, 0x31, 0x32, 0x64, 0x31,
        0x64, 0x21, 0x23, 0x24, 0x21, 0x40, 0x54, 0x31, 0x33, 0x74, 0x66, 0x64,
        0x31, 0x33, 0x52, 0x21, 0x23, 0x66, 0x31, 0x33, 0x52, 0x31, 0x23, 0x54,
        0x23, 0x34, 0x35, 0x68, 0x40, 0x23, 0x24, 0x59, 0x32, 0x34, 0x67, 0x68,
        0x33, 0x25, 0x24, 0x40, 0x33, 0x67, 0x21, 0x32, 0x72, 0x27, 0x32, 0x72,
        0x21, 0x23, 0x66, 0x67, 0x31, 0x23, 0x47, 0x40, 0x34, 0x67, 0x31, 0x32,
        0x66, 0x64, 0x31, 0x77, 0x66, 0x31, 0x23, 0x52, 0x46, 0x21, 0x23, 0x46,
        0x32, 0x34, 0x67, 0x33, 0x34, 0x35, 0x76, 0x63, 0x31, 0x23, 0x46, 0x31,
        0x33
	};
	for (int i = 0; i < length; i++) {
		data[i] ^= 0x63;
	}
	for (int i = 0; i < length; i++) {
		data[i] ^= key[i % sizeof(key)];
	}
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
		char magicword[52];

		if (ReadInFile(filestream, magicword, 52) == false) return false;

		unsigned int size;
		unsigned int compressed;

		if (ReadInFile(filestream, (char*)&size, 4) == false) return false;
		if (ReadInFile(filestream, (char*)&compressed, 4) == false) return false;

		int version = SeperateMagicWord(magicword, 52);

		switch (version)
		{
		default:
			filestream.close();
			return false;
		case 1:
		case 2:
			for (unsigned int i = 0; i < size; i++)
			{
				char key[60];
				if (ReadInFile(filestream, key, 60) == false) return false;
				subfiles.insert(std::map<std::string, KSubfile>::value_type(key, KSubfile(this, filestream, 60 + size * 72)));
			}

			break;
		case 3:
		case 4:
		case 5:
		{
			if (ReadInFile(filestream, (char*)& filetime, 4) == false) return false;
			if (ReadInFile(filestream, (char*)& adler32, 4) == false) return false;
			if (ReadInFile(filestream, (char*)& headersize, 4) == false) return false;
			boost::scoped_array<char> header(new char[headersize]);
			if (ReadInFile(filestream, header.get(), headersize) == false) {
				filestream.close();
				return false;
			}
			XMLDocument doc;
			headerstream.write(magicword, 52);
			headerstream.write((char*)& size, 4);
			headerstream.write((char*)& compressed, 4);
			headerstream.write((char*)& filetime, 4);
			headerstream.write((char*)& adler32, 4);
			headerstream.write((char*)& headersize, 4);
			headerstream.write(header.get(), headersize);
			if (version > 3)
			{
				DecryptHeader(compressed, header.get(), headersize);
			}
			if (version == 5) {
				DecryptAESHeader(header.get(), headersize);
			}
			if (ReadHeader(HEADEROFFSET + headersize, header.get(), headersize) == false) {
				filestream.close();
				headerstream.clear();
				return false;
			}
			break;
		}
		case 6:
		{
			if (ReadInFile(filestream, (char*)&filetime, 4) == false) return false;
			if (ReadInFile(filestream, (char*)&adler32, 4) == false) return false;
			if (ReadInFile(filestream, (char*)&headersize, 4) == false) return false;

			boost::scoped_array<char> header(new char[headersize]);

			if (ReadInFile(filestream, header.get(), headersize) == false)
			{
				filestream.close();
				return false;
			}

			XMLDocument doc;
			headerstream.write(magicword, 52);
			headerstream.write((char*)&size, 4);
			headerstream.write((char*)&compressed, 4);
			headerstream.write((char*)&filetime, 4);
			headerstream.write((char*)&adler32, 4);
			headerstream.write((char*)&headersize, 4);
			headerstream.write(header.get(), headersize);
			DecryptV6Header(header.get(), headersize);

			if (ReadHeader(HEADEROFFSET + headersize, header.get(), headersize) == false)
			{
				filestream.close();
				headerstream.clear();
				return false;
			}
			break;
		}
		}
	}
	return true;
}

bool Komfile::Open(std::stringstream& stream, boost::shared_ptr<KomSession> con, std::string url)
{
	this->filename = url;
	this->session = con;

	headerstream.clear();
	Close();

	unsigned int size;
	unsigned int compressed;
	char magicword[52];

	stream.read(magicword, 52);
	stream.read((char*)& size, 4);
	stream.read((char*)& compressed, 4);

	int version = SeperateMagicWord(magicword, 52);
	if (version < 3)
		return false;

	stream.read((char*)& filetime, 4);
	stream.read((char*)& adler32, 4);
	stream.read((char*)& headersize, 4);

	boost::scoped_array<char> header(new char[headersize]);
	stream.read(header.get(), headersize);

	headerstream.write(magicword, 52);
	headerstream.write((char*)& size, 4);
	headerstream.write((char*)& compressed, 4);
	headerstream.write((char*)& filetime, 4);
	headerstream.write((char*)& adler32, 4);
	headerstream.write((char*)& headersize, 4);
	headerstream.write(header.get(), headersize);

	if (version > 3)
	{
		DecryptHeader(compressed, header.get(), headersize);
	}

	if (ReadHeader(HEADEROFFSET + headersize, header.get(), headersize) == false)
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

	if (session->WriteRemoteData(url, stream, 0, 72) == false)
	{
		return false;
	}

	unsigned int size;
	unsigned int compressed;
	char magicword[52];

	stream.read(magicword, 52);
	stream.read((char*)& size, 4);
	stream.read((char*)& compressed, 4);

	int version = SeperateMagicWord(magicword, 52);
	if (version < 3)
		return false;

	stream.read((char*)& filetime, 4);
	stream.read((char*)& adler32, 4);
	stream.read((char*)& headersize, 4);

	if (session->WriteRemoteData(url, stream, 72, headersize) == false)
		return false;

	XMLDocument doc;

	boost::scoped_array<char> header(new char[headersize]);
	stream.read(header.get(), headersize);

	headerstream.write(magicword, 52);
	headerstream.write((char*)& size, 4);
	headerstream.write((char*)& compressed, 4);
	headerstream.write((char*)& filetime, 4);
	headerstream.write((char*)& adler32, 4);
	headerstream.write((char*)& headersize, 4);
	headerstream.write(header.get(), headersize);

	if (version > 3)
	{
		DecryptHeader(compressed, header.get(), headersize);
	}
	if (ReadHeader(HEADEROFFSET + headersize, header.get(), headersize) == false)
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
		// 원본 데이터가 같으면
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
	// 검증 하고

	if (Komfile::Verify(tmpfilename) == false) // 실패하면
	{
		DeleteFileForce(tmpfilename);
		return false;
	}

	// 성공하면

	// 이름 바꾸자

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
	std::map<std::string, KSubfile>::iterator i = subfiles.begin();

	if (i == subfiles.end())
		return true;

	std::ifstream file(i->second.GetParent()->GetFilename().c_str(), std::ios::in | std::ios::binary);

	if (file.is_open() == false)
		return false;

	while (i != subfiles.end())
	{
		if (AdlerCheckSum::adler32(file, i->second.GetOffset(), i->second.GetCompressedSize()) != i->second.GetAdler32())
		{
			file.close();
			return false;
		}
		i++;
	}
	file.close();
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

void Komfile::DecryptHeader(unsigned int key, char* data, int length)
{
	std::map<unsigned int, INT64>::iterator iter = keymap.find(key);
	if (iter == keymap.end())
		return;

	char keystr[32];
	sprintf_s(keystr, 32, "%lld", iter->second);
	SHA1_CTX sha1;
	SHA1Init(&sha1);
	SHA1Update(&sha1, (unsigned char*)keystr, strlen(keystr));
	unsigned char sha1key[20];
	SHA1Final(sha1key, &sha1);

	CBlowFish blowfish(sha1key, 20);
	blowfish.Decrypt((unsigned char*)data, length);
}

bool Komfile::MannualJoin(Komfile& head, Komfile& local, Komfile& remote)
{
	std::map<std::string, KSubfile>::iterator iter_head = head.subfiles.begin();
	while (iter_head != head.subfiles.end())
	{
		std::map<std::string, KSubfile>::iterator iter_remote = remote.subfiles.find(iter_head->first);
		std::map<std::string, KSubfile>::iterator iter_local = local.subfiles.find(iter_head->first);

		if (iter_local == local.subfiles.end()) // 옛날버전을 위한 안드로메다 하드코딩
		{
			std::string key = iter_head->first;
			std::transform(key.begin(), key.end(), key.begin(), tolower);
			iter_local = local.subfiles.find(key);
		}

		if (iter_remote != remote.subfiles.end()) // 만약 리모트에 파일이 있으면 리모트껄 쓰자
		{
			KSubfile subfile(iter_remote->second);
			subfiles.insert(std::map<std::string, KSubfile>::value_type(iter_head->first, subfile));
		}
		else if (iter_local != local.subfiles.end())
		{
			iter_local->second.GetAdler32();
			if (iter_head->second == iter_local->second)
			{
				KSubfile subfile(iter_local->second);
				subfiles.insert(std::map<std::string, KSubfile>::value_type(iter_head->first, subfile));
			}
			else
			{
				return false;// 이 경우는 답이 없다
			}
		}
		else // 아니면 답이 없다
		{
			return false;
		}
		iter_head++;
	}

	// head 의 헤더를 복사한다
	headerstream.clear();
	int headersize = head.headerstream.tellp();
	if (headersize > 0)
	{
		boost::scoped_array<char> header(new char[headersize]);
		head.headerstream.read(header.get(), headersize);
		headerstream.write(header.get(), headersize);
	}
	return true;
}

int Komfile::LeftOuterJoin(Komfile& remote, Komfile& local)
{
	int re = 0;

	std::map<std::string, KSubfile>::iterator iter_remote = remote.subfiles.begin();
	while (iter_remote != remote.subfiles.end())
	{
		std::map<std::string, KSubfile>::iterator iter_local = local.subfiles.find(iter_remote->first);

		if (iter_local == local.subfiles.end()) // 옛날버전을 위한 안드로메다 하드코딩
		{
			std::string key = iter_remote->first;
			std::transform(key.begin(), key.end(), key.begin(), tolower);
			iter_local = local.subfiles.find(key);
		}

		if (iter_local != local.subfiles.end())// 오른쪽에도 있는 경우만 고른다
		{
			// 만약에 오른쪽 서브파일과 왼쪽이 똑같은 경우 오른쪽을 쓴다
			// 예외적으로 오른쪽 파일 타임이 0 인 경우 - 이 경우는 구버전 콤이다 - 파일타임은 체크하지 않는다

			iter_local->second.GetAdler32();
			if (iter_remote->second == iter_local->second)
			{
				subfiles.insert(std::map<std::string, KSubfile>::value_type(iter_remote->first, iter_local->second));
			}
			else
			{
				subfiles.insert(std::map<std::string, KSubfile>::value_type(iter_remote->first, iter_remote->second));
				re++;
			}
		}
		else
		{
			re++;
			// Left Outer Join 인 경우 Right 에 없더라도 저장하자
			subfiles.insert(std::map<std::string, KSubfile>::value_type(iter_remote->first, iter_remote->second));
		}
		iter_remote++;
	}

	// left 의 헤더를 복사한다
	headerstream.clear();
	int headersize = remote.headerstream.tellp();
	if (headersize > 0)
	{
		boost::scoped_array<char> header(new char[headersize]);
		remote.headerstream.read(header.get(), headersize);
		headerstream.write(header.get(), headersize);
	}

	return re;
}

void Komfile::AddKey(INT64 key)
{
	std::stringstream stream;
	stream << key;
	unsigned int adler = AdlerCheckSum::adler32(stream, 0, stream.tellp());

	keymap.insert(std::map<unsigned int, INT64>::value_type(adler, key));
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
	{
		return false;
	}

	return true;
}