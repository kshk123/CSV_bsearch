#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 10000

int searchKey(char ** keyArr, const char * key, int lineNum)
{
	int mid = 0;
	int begin = 0;
	int end = lineNum;

	while(begin <= end) /* Implementing binary search */
	{
		mid = begin + ((end - begin)/2) ;
		int ret = strcmp(keyArr[mid], key);
		if(0 == ret) /* Key found, return the location */
		{
			return mid;
		}
		else
		{
			if(ret < 1)
				begin = mid + 1;
			else
				end = mid - 1;
		}
	}

	printf("Key %s is not present in input file.\n", key);
	return -1;
}


void readFileNFindKey(const char * key, const char * inFile)
{
	char * keyArr[MAX_KEYS]; /* Array to store keys */
	int offsetArr[MAX_KEYS]; /* Array to store offset of start of line */
	char * line = NULL;
	char * tok = NULL;
    size_t len = 0;
    ssize_t read;
	int lineNum = 0;
	int offset = 0;
	int i = 0;

	memset(offsetArr, 0, MAX_KEYS * sizeof(int));
	FILE * fp = fopen(inFile, "r");
	if(NULL == fp)
	{
		printf("Unable to open input file.\n");
		return;
	}	
	/* Read input file line by line, and get the first token.
	   Store the first token in an in-memory array, 
	   along with the byte offset of the start of the line in another array, at the same index.
	*/
	while ((read = getline(&line, &len, fp)) != -1) 
	{
		if(strstr(line, ",")) 
		{
			tok = strtok(line, ","); /* store the first token */
			int tokLen = strlen(tok);
			keyArr[lineNum] = (char *)malloc((tokLen + 1) * sizeof(char));
			strcpy(keyArr[lineNum],tok);
		}
		else /* The line contains only one token, hence insert the whole line*/
		{
			keyArr[lineNum] = (char *)malloc((read + 1) * sizeof(char));
			strncpy(keyArr[lineNum], line, read -1); /* -1 is to remove the trailing '\n' */
		}

		offsetArr[lineNum] = offset;
		lineNum++;
		offset += read;
    }

	lineNum -= 1;
	/* Now search for the key in the stored array and print the line */
	int loc = searchKey(keyArr, key, lineNum);
	if(loc >= 0) /* Match found, not output the line */
	{
		fseek(fp, offsetArr[loc], SEEK_SET);
		getline(&line, &len, fp);
		printf("%s", line);
	}

    fclose(fp);
    if (line)
        free(line);

	for(i = 0; i <= lineNum; i++) /* cleanup the memory allocated for key array */
	{
		free(keyArr[i]);
	}
}

int main(int argc, char ** argv)
{
	if(argc != 4)/* Check for number of arguments */
	{
		printf("Incorrect number of commands.\n");
		return 1;
	}

	if(strcmp(argv[1], "--key"))
	{
		printf("First argument should be '--key'.\n");
        return 1;
	}

	const char * key = argv[2];
	const char * inFile  = argv[3];

	readFileNFindKey(key, inFile); 
	
	return 0;
}

