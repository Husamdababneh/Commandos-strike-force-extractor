struct CSFFBSHeader {
	char Tag[6];
	padding[2];
	u32 Version; // ?? maybe
	u32 NumberOfItems; // * 12) + 24= Offset Of Where Data Begins -v
	u32 StringEntryCount;
	u32 StringValueCount;
};

union Data_Value_Unioin {
	// Data 
	u32 IntegerValue;
	float FloatValue;
	// String 
	u32 StringIndex;
	// List 
	u32 InnerItemCount;
};
struct CSFFBString {
	u32 size;
	if( size > 0){
        char string[];
    }
};

enum CSFFBSDataType : u16 {
	CSF_HEADER        = 0,
	CSF_COLLECTION    = 1,
	CSF_LIST          = 2,
	CSF_INT           = 3,
	CSF_FLOAT         = 4,
	CSF_STRING        = 5 
};

struct CSFFBSData
{
	u32 Indicator;
	Data_Value_Unioin value;
	u16 ItemStringIndex;
	CSFFBSDataType Type;
};

struct CSFFBSFile {
	CSFFBSHeader header;
	CSFFBSData data[header.NumberOfItems];
	CSFFBString keys[header.StringEntryCount];
	CSFFBString string_table[header.StringValueCount];
};

CSFFBSFile file @ 0x0;
//CSFFBString file_strings[file.header.StringEntryCount] @ 0x000033F0;
//CSFFBString strings_values[file.header.StringValueCount] @ 0x00003498;

