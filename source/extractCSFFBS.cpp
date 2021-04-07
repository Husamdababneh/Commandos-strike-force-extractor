
void ExtractCSFFBS(const char *filepath)
{
	u8* filedata;
	if (!read_entire_file(filepath, &(void*)filedata)){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}
	CSFFBSHeader *Header = (CSFFBSHeader*)filedata;
	printf("Tag           : %s\n", Header->Tag);
	printf("Version       : %d\n", Header->Version);
	printf("NumberOfItems : %d\n", Header->NumberOfItems);
	printf("Entries Count : %d\n", Header->StringEntryCount );
	printf("String Count  : %d\n", Header->StringValueCount);
	printf("Number Count  : %d\n", Header->NumberOfItems - Header->StringValueCount );
	printf("--------------------------------------\n");
	
	u32 string_location = (Header->NumberOfItems * 12 + 24);
	
	//handle strings
	u32 strings_count = Header->StringEntryCount + Header->StringValueCount;
	u8** strings = (u8**)malloc(sizeof(u8*) * strings_count );
	u32 strings_offset = (Header->NumberOfItems * 12) + sizeof(CSFFBSHeader);
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
	u8** values_strings  = &strings[Header->StringEntryCount];
	
	CSFFBSData* data = (CSFFBSData*)(filedata + sizeof(CSFFBSHeader));
	{
		for(u32 i = 0; i < Header->NumberOfItems; i++){
			CSFFBSData d = data[i];
			switch(d.Type)
			{
				case CSF_HEADER: 
				{
					printf("%s\n", entries_strings[d.ItemStringIndex]);
					break;
				}
				case CSF_INT: 
				{
					printf("%d\n", d.IntegerValue);
					break;
				}
				case CSF_FLOAT: 
				{
					printf("%f\n", d.FloatValue);
					break;
				}
				case CSF_STRING: 
				{
					printf("%s\n", values_strings[d.StringIndex]);
					break;
				}
				case CSF_COLLECTION: 
				{
					//print_coll(&data[i], i + 1, d.InnerItemCount);
					printf("(\n", entries_strings[d.StringIndex]);
					break;
				}
				case CSF_LIST: 
				{
					printf("[\n", entries_strings[d.StringIndex]);
					break;
				}
				default:
				{
					assert(false && "What is this");
				}
			}
			
		}
	}
	delete filedata;
}
