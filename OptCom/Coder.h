#pragma once
#include <stdio.h>
#include "CPP/Common/MyWindows.h"

#include "CPP/Common/Defs.h"
#include "CPP/Common/MyInitGuid.h"

#include "CPP/Common/IntToString.h"
#include "CPP/Common/StringConvert.h"

#include "CPP/Windows/DLL.h"
#include "CPP/Windows/FileDir.h"
#include "CPP/Windows/FileFind.h"
#include "CPP/Windows/FileName.h"
#include "CPP/Windows/NtCheck.h"
#include "CPP/Windows/PropVariant.h"
#include "CPP/Windows/PropVariantConv.h"

#include "CPP/7zip/Common/FileStreams.h"

#include "CPP/7zip/Archive/IArchive.h"

#include "CPP/7zip/IPassword.h"
#include "C/7zVersion.h"



// Tou can find the list of all GUIDs in Guid.txt file.
// use another CLSIDs, if you want to support other formats (zip, rar, ...).
// {23170F69-40C1-278A-1000-000110070000}

DEFINE_GUID(CLSID_CFormat7z,
	0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);
DEFINE_GUID(CLSID_CFormatXz,
	0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x0C, 0x00, 0x00);

#define CLSID_Format CLSID_CFormat7z
// #define CLSID_Format CLSID_CFormatXz

using namespace NWindows;
using namespace NFile;
using namespace NDir;
//----
#define kDllName "7zra.dll"
// #define kDllName "7z.dll"

static const char *kCopyrightString = "\n7-Zip " MY_VERSION
" ("  kDllName " client) "
MY_COPYRIGHT " " MY_DATE "\n";

static const char *kHelpString =
"Usage: Client7z.exe [a | l | x ] archive.7z [fileName ...]\n"
"Examples:\n"
"  Client7z.exe a archive.7z f1.txt f2.txt  : compress two files to archive.7z\n"
"  Client7z.exe l archive.7z   : List contents of archive.7z\n"
"  Client7z.exe x archive.7z   : eXtract files from archive.7z\n";

static AString FStringToConsoleString(const FString &s);

static FString CmdStringToFString(const char *s);
static void PrintString(const UString &s);

static void PrintString(const AString &s);

static void PrintNewLine();

static void PrintStringLn(const AString &s);
static void PrintError(const char *message, const FString &name);

static void PrintError(const AString &s);
static HRESULT IsArchiveItemProp(IInArchive *archive, UInt32 index, PROPID propID, bool &result);
static HRESULT IsArchiveItemFolder(IInArchive *archive, UInt32 index, bool &result);
static const wchar_t *kEmptyFileAlias = L"[Content]";


//////////////////////////////////////////////////////////////
// Archive Open callback class


class CArchiveOpenCallback :
	public IArchiveOpenCallback,
	public ICryptoGetTextPassword,
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

		STDMETHOD(SetTotal)(const UInt64 *files, const UInt64 *bytes);
	STDMETHOD(SetCompleted)(const UInt64 *files, const UInt64 *bytes);

	STDMETHOD(CryptoGetTextPassword)(BSTR *password);

	bool PasswordIsDefined;
	UString Password;

	CArchiveOpenCallback() : PasswordIsDefined(false) {}
};

//////////////////////////////////////////////////////////////
// Archive Extracting callback class

static const char *kTestingString = "Testing     ";
static const char *kExtractingString = "Extracting  ";
static const char *kSkippingString = "Skipping    ";

static const char *kUnsupportedMethod = "Unsupported Method";
static const char *kCRCFailed = "CRC Failed";
static const char *kDataError = "Data Error";
static const char *kUnavailableData = "Unavailable data";
static const char *kUnexpectedEnd = "Unexpected end of data";
static const char *kDataAfterEnd = "There are some data after the end of the payload data";
static const char *kIsNotArc = "Is not archive";
static const char *kHeadersError = "Headers Error";

class CArchiveExtractCallback :
	public IArchiveExtractCallback,
	public ICryptoGetTextPassword,
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

		// IProgress
		STDMETHOD(SetTotal)(UInt64 size);
	STDMETHOD(SetCompleted)(const UInt64 *completeValue);

	// IArchiveExtractCallback
	STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream **outStream, Int32 askExtractMode);
	STDMETHOD(PrepareOperation)(Int32 askExtractMode);
	STDMETHOD(SetOperationResult)(Int32 resultEOperationResult);

	// ICryptoGetTextPassword
	STDMETHOD(CryptoGetTextPassword)(BSTR *aPassword);

private:
	CMyComPtr<IInArchive> _archiveHandler;
	FString _directoryPath;  // Output directory
	UString _filePath;       // name inside arcvhive
	FString _diskFilePath;   // full path to file on disk
	bool _extractMode;
	struct CProcessedFileInfo
	{
		FILETIME MTime;
		UInt32 Attrib;
		bool isDir;
		bool AttribDefined;
		bool MTimeDefined;
	} _processedFileInfo;

	COutFileStream *_outFileStreamSpec;
	CMyComPtr<ISequentialOutStream> _outFileStream;

public:
	void Init(IInArchive *archiveHandler, const FString &directoryPath);

	UInt64 NumErrors;
	bool PasswordIsDefined;
	UString Password;
	//-----
	UInt64 FileSize;

	CArchiveExtractCallback() : PasswordIsDefined(false) {}
};

//////////////////////////////////////////////////////////////
// Archive Creating callback class

struct CDirItem
{
	UInt64 Size;
	FILETIME CTime;
	FILETIME ATime;
	FILETIME MTime;
	UString Name;
	FString FullPath;
	UInt32 Attrib;

	bool isDir() const { return (Attrib & FILE_ATTRIBUTE_DIRECTORY) != 0; }
};

class CArchiveUpdateCallback :
	public IArchiveUpdateCallback2,
	public ICryptoGetTextPassword2,
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP2(IArchiveUpdateCallback2, ICryptoGetTextPassword2)

		// IProgress
		STDMETHOD(SetTotal)(UInt64 size);
	STDMETHOD(SetCompleted)(const UInt64 *completeValue);

	// IUpdateCallback2
	STDMETHOD(GetUpdateItemInfo)(UInt32 index,
		Int32 *newData, Int32 *newProperties, UInt32 *indexInArchive);
	STDMETHOD(GetProperty)(UInt32 index, PROPID propID, PROPVARIANT *value);
	STDMETHOD(GetStream)(UInt32 index, ISequentialInStream **inStream);
	STDMETHOD(SetOperationResult)(Int32 operationResult);
	STDMETHOD(GetVolumeSize)(UInt32 index, UInt64 *size);
	STDMETHOD(GetVolumeStream)(UInt32 index, ISequentialOutStream **volumeStream);

	STDMETHOD(CryptoGetTextPassword2)(Int32 *passwordIsDefined, BSTR *password);

public:
	CRecordVector<UInt64> VolumesSizes;
	UString VolName;
	UString VolExt;

	FString DirPrefix;
	const CObjectVector<CDirItem> *DirItems;

	bool PasswordIsDefined;
	UString Password;
	bool AskPassword;

	bool m_NeedBeClosed;

	FStringVector FailedFiles;
	CRecordVector<HRESULT> FailedCodes;

	//------
	UInt64 FileSize;

	CArchiveUpdateCallback() : PasswordIsDefined(false), AskPassword(false), DirItems(0) {};

	~CArchiveUpdateCallback() { Finilize(); }
	HRESULT Finilize();

	void Init(const CObjectVector<CDirItem> *dirItems)
	{
		DirItems = dirItems;
		m_NeedBeClosed = false;
		FailedFiles.Clear();
		FailedCodes.Clear();
	}
};

int CoderFunc(int numArgs, const char *args[]);
int EnCoderFunc(char *FileName);
int DeCoderFunc();