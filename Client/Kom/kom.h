#pragma once

#include "stdafx.h"
#include "XmlWraper.h"
#include "adler32.h"
#include "session.h"

#define HEADEROFFSET 72
#define KOMOPEN_MAXRETRY 4

extern void ResetReadOnly(std::string path);
extern bool DeleteFileForce(std::string filename);
extern bool DeleteDirForce(std::string dirname);
extern bool IsFileExists(std::string filename);

class Komfile;

class KSubfile
{
public:
	enum EAlgorithm
	{
		infilate = 0,
		lzma = 1
	};

	KSubfile()
		:compressedsize(0), iscalcadler(true), adler32(0), size(0), parentkom(NULL), offset(0), filetime(0)
	{

	}
	KSubfile(Komfile* parent, std::ifstream& stream, int headersize); // �� ���� �޿��� �д� �Լ�		
	KSubfile(Komfile* parent, xmlNode* node, int* offset); // URL �Ź���


	KSubfile(const KSubfile& src)
		:parentkom(src.parentkom), filetime(src.filetime), adler32(src.adler32), size(src.size), compressedsize(src.compressedsize), algorithm(src.algorithm), offset(src.offset), iscalcadler(src.iscalcadler)
	{}
	~KSubfile() {};

	bool operator == (const KSubfile& r)
	{
		if (r.filetime == 0)
			return (adler32 == r.adler32 && algorithm == r.algorithm && compressedsize == r.compressedsize);

		return (adler32 == r.adler32 && algorithm == r.algorithm && compressedsize == r.compressedsize && filetime == r.filetime);
	}

	KSubfile& operator= (const KSubfile& r)
	{
		adler32 = r.adler32;
		algorithm = r.algorithm;
		compressedsize = r.compressedsize;
		parentkom = r.parentkom;
		size = r.size;
		offset = r.offset;
		iscalcadler = r.iscalcadler;
		if (r.filetime != 0)
			filetime = r.filetime;
		return *this;
	}

	bool WriteCompressed(std::ostream& stream);
	bool WriteCompressed(std::ostream& stream, NETPROGRESS_CALLBACK progress, KCSLockType<SProgressInfo>& progressinfo);
	bool WriteDecompressed(std::ostream& stream);
	bool WriteDecompressedFile(const std::string fileName);
	bool WriteCompressed(char* data);
	bool WriteDecompressed(char* data);

	bool GetFileData(std::string& _filename, unsigned int& _offset, unsigned int& _compressedsize, unsigned int& _size);

	unsigned int GetFileTime() { return filetime; }
	void SetFileTime(unsigned int ft) { filetime = ft; } // �����ؼ� �� ����Ѵ�.

	unsigned int GetAdler32();
	unsigned int GetSize() { return size; }
	unsigned int GetCompressedSize() { return compressedsize; }
	int GetAlgorithm() { return algorithm; }

	Komfile* GetParent() { return parentkom; }
	void AddCompressedSize(int s) { compressedsize += s; }

	int GetOffset() { return offset; }
private:

	unsigned int filetime;
	unsigned int adler32;
	unsigned int size;
	unsigned int compressedsize;
	int algorithm;

	unsigned int offset;
	Komfile* parentkom;
	bool iscalcadler;
};


struct SKomWriteInfo :public boost::noncopyable
{

	SKomWriteInfo(NETPROGRESS_CALLBACK prog, NETPROGRESS_CALLBACK prog2, KCSLockType<SProgressInfo>& proginfo)
		:progress(prog), progress2(prog2), progressinfo(proginfo)
	{
	}

	bool IsStop()
	{
		KCSLOCK_BLOCK(progressinfo)
		{
			return progressinfo.isstoped;
		}
		return false;
	}

	void Read(int len)
	{
		KCSLOCK_BLOCK(progressinfo)
		{
			progressinfo.Read(len);

			if (progress)
				progress(progressinfo);
		}
	}

	void GetProgressInfo(SProgressInfo& sProgressinfo)
	{
		KCSLOCK_BLOCK(progressinfo)
		{
			progressinfo.GetProgressInfo(sProgressinfo);
		}
	}

	NETPROGRESS_CALLBACK progress;
	NETPROGRESS_CALLBACK progress2;
	KCSLOCK_DECLARE(SProgressInfo&, progressinfo);
};


class Komfile :public boost::noncopyable
{
public:
	static bool Verify(std::string filename);

	Komfile() { Close(); };
	Komfile(const Komfile& r)
		:subfiles(r.subfiles), filetime(r.filetime), adler32(r.adler32), headersize(r.headersize)
	{};

	~Komfile()
	{
		Close();
	};

	int LeftOuterJoin(Komfile& left, Komfile& right);
	bool MannualJoin(Komfile& head, Komfile& local, Komfile& remote);

	bool Open(boost::shared_ptr<KomSession> con, std::string url);
	bool Open(std::stringstream& stream, boost::shared_ptr<KomSession> con, std::string url);
	bool Open(std::string filename);
	bool Open(std::string filename, std::ifstream& filestream);
	bool Save(std::string filename, SKomWriteInfo& writeinfo);
	void Close();

	unsigned int GetFileTime();
	unsigned int GetAdler32() { return adler32; }
	unsigned int GetHeaderSize() { return headersize; }
	unsigned int GetTotalSize();

	int GetFileCount() { return subfiles.size(); }

	bool Verify();

	std::string GetFilename() { return filename; }
	KomSession* GetSession() { return session.get(); }
	std::map<std::string, KSubfile>& GetSubfiles() { return subfiles; }

	KSubfile GetSubfile(std::string filename);
	bool IsExist(std::string filename);

protected:
	int SeperateMagicWord(char* data, int length);
	bool ReadHeader(int offset, char* data, int lenfth);
	bool ReadInBuffer(char* data, int size);

	std::map<std::string, KSubfile> subfiles;
	unsigned int filetime;
	unsigned int adler32;
	unsigned int headersize;

	std::stringstream headerstream;

	std::string filename;
	boost::shared_ptr<KomSession> session;

	size_t bufPos;
	size_t bufSize;
	char* buf;
	bool isBuffered;
};
