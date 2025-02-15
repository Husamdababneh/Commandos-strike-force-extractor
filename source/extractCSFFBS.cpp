
// TODO: clean this shit up

u8* getType(FBSType type);
#define CSFFBS_PRTINTER_PROC(name) u64 (name)(const CSFFBSFile* file, CSFFBSData* data)
typedef u64 (*CSFFBS_PRTINTER)(const CSFFBSFile*, CSFFBSData*);
CSFFBS_PRTINTER_PROC(CSFFBS_STUB)
{
    // TODO: Assert here
    
    printf("[%s]: Not Implemented Yet\n", getType(data->type));
    return 0;
}

CSFFBS_PRTINTER_PROC(print_list);
CSFFBS_PRTINTER_PROC(print_string);
CSFFBS_PRTINTER_PROC(print_int);
CSFFBS_PRTINTER_PROC(print_float);
CSFFBS_PRTINTER_PROC(print_collection);
CSFFBS_PRTINTER_PROC(print_key);

CSFFBS_PRTINTER functions[] = {
    print_key,        //CSF_KEY
    print_collection, //CSF_COLLECTION
    print_list,       //CSF_LIST      
    print_int,        //CSF_INT       
    print_float,      //CSF_FLOAT     
    print_string      //CSF_STRING    
};


static inline
u8* getHeaderKey(const CSFFBSFile* file, u64 index)
{
    auto keys = file->keys;
    bool keys_minus_one = file->header->padding[1] != 127;
    // return keys_minus_one ? keys[index - 1]: keys[index];
    return keys[index];
}



void print_indent(u32 indent)
{
    for(int i = 0; i < indent; ++i)printf(" ");
};

u8* getType(FBSType type)
{
    switch(type)
    {
      case CSF_KEY:        return (u8*)"CSF_KEY";
      case CSF_COLLECTION: return (u8*)"CSF_COLLECTION";
      case CSF_LIST:       return (u8*)"CSF_LIST";
      case CSF_INT:        return (u8*)"CSF_INT";
      case CSF_FLOAT:      return (u8*)"CSF_FLOAT";
      case CSF_STRING:     return (u8*)"CSF_STRING";
    }
    return (u8*)"Unknown Type";
}


CSFFBS_PRTINTER_PROC(print_collection)
{
    assert(data->type == CSF_COLLECTION);
    u64 result = 1;
    if (data->ItemStringIndex != 0xFFFF)
    {
        u8* name = getHeaderKey(file, data->ItemStringIndex);
        printf("%s ( ", name);
        for(int i = 1; i <= data->InnerItemCount; i++)        
        {
            auto it = data + i;
            functions[it->type](file, it);
            printf(" ");
            result++;
        }
        printf(" )\n");
        
    }else
    {
        exit(-1);
    }
    return result;
}


CSFFBS_PRTINTER_PROC(print_key)
{
    assert(data->type == CSF_KEY);
    printf("%s\t", getHeaderKey(file, data->ItemStringIndex));
    return 0;
}


CSFFBS_PRTINTER_PROC(print_float)
{
    assert(data->type == CSF_FLOAT);
    printf("%f", data->FloatValue);
    return 1;
}

CSFFBS_PRTINTER_PROC(print_int)
{
    assert(data->type == CSF_INT);
    printf("%d", data->IntegerValue);
    return 1;
}

CSFFBS_PRTINTER_PROC(print_string)
{
    assert(data->type == CSF_STRING);
    u8* value =  file->string_table[data->StringIndex];
    if (value == NULL) value = (u8*)"";
    printf("\"%s\"", value);
    
    return 1;
}


CSFFBS_PRTINTER_PROC(print_list)
{
    CSFFBSData* list = data;
    assert(list->type == CSF_LIST);

    u64 result = 0;
    if (list->ItemStringIndex != 0xFFFF)
    {
        u8* list_name = getHeaderKey(file, list->ItemStringIndex);
        printf("%s\n", list_name);
    }
    printf("[\n");
    CSFFBSData* innerDataIt = data + result + 1;
    for(u32 i = 0; i < list->InnerItemCount; i++)
    {
        result++;
        auto iresult = functions[innerDataIt->type](file, innerDataIt);
        if(iresult == 0)
        {
            innerDataIt +=1;
            iresult = functions[innerDataIt->type](file, innerDataIt);
            printf("\n");
        }
        innerDataIt = innerDataIt + iresult;
            
    }
    printf("]\n");

    return result;
}


void ExtractCSFFBS(const char *filepath)
{
	void* filedataD;
	if (!read_entire_file(filepath, &filedataD)){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}

    u8* filedata = (u8*)filedataD;

	CSFFBSHeader *header = (CSFFBSHeader*)filedata;
    if (memcmp(FSB_MAGIC, header->Tag, sizeof(FSB_MAGIC)) != 0)
    {
        printf("File [%s] is not a valid CSF-FBS file\n", filepath);
        exit(-1);
    }
	printf("Tag           : %s\n", header->Tag);
	printf("Version       : %d\n", header->Version);
    // printf("Padding[0]    : %x\n", header->padding[0]);
    // printf("Padding[1]    : %x\n", header->padding[1]);
	printf("#Nodes        : %d\n", header->NumberOfItems);
	printf("Keys Count    : %d\n", header->StringEntryCount );
	printf("String Count  : %d\n", header->StringValueCount);
	printf("--------------------------------------\n");
	
	u32 string_location = (header->NumberOfItems * 12 + 24);
	
	//handle strings
	u32 strings_count = header->StringEntryCount + header->StringValueCount;
	u8** strings = (u8**)malloc(sizeof(u8*) * strings_count );
	u32 strings_offset = (header->NumberOfItems * 12) + sizeof(CSFFBSHeader);
	{
		u8* cursor = filedata + strings_offset;
		for(u32 i = 0; i < strings_count; i++)
		{
			u32 strsize = *(u32*)(cursor); // this is the size of the string + 1 (null)
			
			if (strsize == 0) strings[i] = nullptr;
			else strings[i] = (cursor + 4);
			
			cursor = cursor + 4 + strsize;
		}
	}
    
	u8** entries_strings = strings;
	u8** values_strings  = &strings[header->StringEntryCount];
	CSFFBSData* data = (CSFFBSData*)(filedata + sizeof(CSFFBSHeader));

    CSFFBSFile file = {};
    file.header       = header;
    file.data         = data;            
    file.keys         = entries_strings;
    file.string_table = values_strings;

    functions[file.data->type](&file,file.data);
	delete filedata;
}
