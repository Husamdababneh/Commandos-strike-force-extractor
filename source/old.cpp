std::atomic<int> progress = 0;
std::atomic<int> maxNum = 100;

class cProfiler
{
	private:
    std::chrono::time_point<std::chrono::steady_clock> m_start;
    std::chrono::duration<float>& m_duration;
	public:
    cProfiler(std::chrono::duration<float>& duration):
	m_duration(duration)
    {
        m_start = std::chrono::steady_clock::now();
    }
	
    ~cProfiler()
    {
        m_duration = (std::chrono::steady_clock::now() - m_start);
		std::cout << "Timer : " << static_cast<float>(m_duration.count()) << "\n";
    }
};

#if 0
bool ExtractPakFiles(const char * filepath)
{
	std::chrono::duration<float> timer;
	cProfiler c(timer);
	
	std::cout << "Opening  " << filepath << "\n";
	std::cout << "Sizeof(PakHeader)" << sizeof(PakHeader) << "\n";
	std::fstream file(filepath, std::fstream::in | std::fstream::binary);
	
	if (!file.is_open())
		LOGANDEXIT("Error : cannot open " << filepath << "\n");
	
	int filesize = std::filesystem::file_size(filepath);
	PakHeader pak = { 0 };
	int filesOffset;
	int maximumFileSize = 0;
	file.read((char*)&pak, sizeof(PakHeader));
	std::cout << "File Tag = ";
	std::cout.write(pak.Tag,4) << "\n";
	std::cout << "IDK = " << pak.IDK << "\n";
	std::cout << "Version = " << pak.Version << "\n";
	std::cout << "ResNum   = " << pak.ResNum << "\n";
	
	FilesData* filesData = new FilesData[pak.ResNum];
	long lastitem = 0;
	for (int a = 0; a < pak.ResNum; a++)
	{
		auto& filedata = filesData[a];
		char filename[120];
		memset(filename, 0, 120); 
		
		char ch = 0;
		int size = 0;
		
		while((ch = file.get()) != '\0')
			filedata.Filename[size++] = ch;
		
		filedata.Filename[size] = '\0';
		
		file.read((char*)&filedata.ROFF, sizeof(long));
		file.read((char*)&filedata.Size, sizeof(long));
		file.read((char*)&filedata.Unknown1, sizeof(long));
		file.read((char*)&filedata.WhereItIs, sizeof(long));
		
		// std::cout << "roff: " << filedata.ROFF;
		// std::cout << "\tsize:" << std::setw(8) << filedata.Size;
		// std::cout << " v1: "  << std::hex  << filedata.Unknown1;
		// std::cout << "\tv2: "  << std::hex  << (int)filedata.WhereItIs << std::dec;
		// std::cout << "\tFilename: " << filedata.Filename;
		// std::cout << "\n";
		
		
		lastitem = filedata.Unknown1;
		
		if (filedata.Size > maximumFileSize)
			maximumFileSize = filedata.Size;
	}	
	//return 0;
	
	maxNum = pak.ResNum;
	char* data = new char[maximumFileSize];
	for (int a = 0; a < pak.ResNum; a++)
	{
		std::filesystem::path path{ "./Data/" };
		path /= std::filesystem::path(filepath).stem();
		path /= filesData[a].Filename;
		std::filesystem::create_directories(path.parent_path());
		std::fstream outfile(path, std::fstream::out | std::fstream::binary);
		
		if (((char*)&pak.Tag)[3] == 'A')
		{
			file.read(data, filesData[a].Size);
		}
		else
		{
			long PackedBlockSize, UnpackedBlockSize;
			int size = filesData[a].Size;
			char junk[sizeof(long)];
			int offset = 0;
			
			// What i found was that inflate has around 4:1 ratio of compression
			// for avarage files
			constexpr int MaxPackedBlockSize = 4096 + 1024;
			constexpr int MaxUnpackedBlockSize = MaxPackedBlockSize * 4;
			char packed[MaxPackedBlockSize], unpacked[MaxUnpackedBlockSize];
			// @Incomplete(Husam):Performance and more general way ??.
			while(1){
				if (size <= 0)
				{
					file.read(junk, sizeof(long));
					break;
				}
				file.read((char*)&PackedBlockSize, sizeof(long));
				file.read((char*)&UnpackedBlockSize, sizeof(long));
				
				assertm(MaxPackedBlockSize >= PackedBlockSize,
						"PackedBlockSize is bigger than you think");
				assertm(MaxUnpackedBlockSize >= UnpackedBlockSize,
						"UnpackedBlockSize is bigger than you think");
				
				file.read(packed, PackedBlockSize);
				
				decompress(packed, PackedBlockSize, unpacked, UnpackedBlockSize);
				memcpy(data + offset, unpacked, UnpackedBlockSize);
				
				size -= UnpackedBlockSize;
				offset += UnpackedBlockSize;
			}
		}
		outfile.write(data, filesData[a].Size);
		outfile.close();
		
	}
	std::cout << "Finished: " << filepath  << "\n";
	file.close();
	delete[] filesData;
	return true;
}

void ExtractAllFiles(const char * filepath)
{
	std::filesystem::path path{ filepath };
	if (!std::filesystem::is_directory(filepath))
	{
		std::cout << "not direcotry\n";
		return;
	}
	
	int rescount = 0;
	for(auto& p: std::filesystem::recursive_directory_iterator(path))
		if(!std::filesystem::is_directory(p))
		ExtractPakFiles(p.path().string().c_str());
}
#endif
// We Have script for that <3
/*
  RPC FileFormat
  
  4  - string_identifier == 16
  20 - unknown
  4  - models count 
  8  - unknown
  4  - objects count 

  (12*4             +    4          +      4 ) * objects
  transformation        parent     +      padding? 

*/

bool RPCConvert(const char * filepath)
{
	std::cout << "Opening  " << filepath << "\n";
	std::fstream file(filepath, std::fstream::in | std::fstream::binary);
	
	if (!file.is_open())
		LOGANDEXIT("Error : cannot open " << filepath << "\n");
	
	unsigned long string_identifier;
	file.read((char*)&string_identifier, sizeof(long));
	if(string_identifier != 16)
		LOGANDEXIT("Error : Not Vaild RPC File : string_identifier is not 16\n");
	
	char unknown1[20] = {0};
	file.read(unknown1, 20);
	
	long ModelsCount;
	file.read((char*)&ModelsCount, sizeof(long));
	
	char unknown2[8]= {0};
	file.read(unknown1, 8);
	
	long ObjectsCount;
	file.read((char*)&ObjectsCount, sizeof(long));
	
	void* objects = new char[ObjectsCount];
	for(int i = 0; i < ObjectsCount; i++)
	{
		float f[16];
		for(int j = 0; j < 12; j++)
		{
			file.read((char*) &f[j], sizeof(float)); 
		}
	}
	
	delete objects;
	
	std::cout << "Model count : " << ModelsCount << "\n";
	std::cout << "Object  count : " << ObjectsCount << "\n";
	file.close();
	return true;
}


void CompressPAC(const char * folderPath)
{
	std::filesystem::path path{ folderPath };
	if (!std::filesystem::is_directory(folderPath))
	{
		std::cout << "not direcotry";
		return;
	}
	
	int rescount = 0;
	for(auto& p: std::filesystem::recursive_directory_iterator(path))
		if(!std::filesystem::is_directory(p))
		rescount++;
	
	std::cout << "Res Count = " << rescount << "\n";
	std::fstream outfile ("GlobalES.pak", std::fstream::out | std::fstream::binary);
	
	PakHeader header;// = {"PAKA", 5, 1, rescount};
	strcpy(header.Tag, "PAKA");
	header.IDK = 5;
	header.Version = 1;
	header.ResNum =rescount;
	outfile.write((char*)&header, sizeof(header));
	
	long V2 = 0;
	long offset = 13;
	
	for(auto& p: std::filesystem::recursive_directory_iterator(path))
	{
		char filename[120];
		
		if(!std::filesystem::is_directory(p))
		{
			long size = std::filesystem::file_size(p);
			
			size_t filenameSize = strlen(p.path().string().c_str()) - strlen(folderPath);
			strcpy(filename, p.path().string().c_str() + strlen(folderPath) + 1);
			
			outfile.write(filename, filenameSize);
			outfile.write((char*)&offset, sizeof(long)); // offset ?? 
			outfile.write((char*)&size, sizeof(long)); // size
			outfile.write((char*)&V2, sizeof(long)); // unknown
			outfile.write((char*)&V2, sizeof(long)); // unknown 2
			offset+=size;
		}
	}
	
	for(auto& p: std::filesystem::recursive_directory_iterator(path))
	{
		
		if(!std::filesystem::is_directory(p))
		{
			long size = std::filesystem::file_size(p);
			std::fstream myfile(p.path().string().c_str(),
								std::ios::in | std::ios::binary);
			char* data = new char[size];
			myfile.read(data, size);
			outfile.write(data, size);
			delete[] data;
		}
	}
	outfile.close();
}
