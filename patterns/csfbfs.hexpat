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
	if (size > 0) { // this just means an empty string 
		char str[];
	}
};
struct CSFFBSData
{
	u32 Indicator;
	Data_Value_Unioin value;
	u16 ItemStringIndex;
	u16 Type;
};

struct CSFFBSFile {
	CSFFBSHeader header;
	CSFFBSData data[header.NumberOfItems];
	CSFFBString stings[header.StringEntryCount];
	CSFFBString stings2[header.StringValueCount];
};

CSFFBSFile file @ 0x0;

