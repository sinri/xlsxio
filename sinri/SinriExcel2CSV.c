#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/xlsxio_read.h"

#ifndef DEBUG_MODE_BY_SINRI
#define DEBUG_MODE_BY_SINRI 0
#endif

int main (int argc, char* argv[])
{
  char* filename=NULL;
  char* given_sheetname=NULL;
  int i=0;
  if(argc>1){
    filename=argv[1];
    if(argc>2){
      given_sheetname=argv[2];
    }
  }else{
    fprintf(stderr, "No Excel file found\n");
    return 1;
  }

  xlsxioreader xlsxioread;
  //open .xlsx file for reading
  if ((xlsxioread = xlsxioread_open(filename)) == NULL) {
    fprintf(stderr, "Error opening .xlsx file\n");
    return 1;
  }

  //list available sheets
  xlsxioreadersheetlist sheetlist;
  const char* sheetname;
  int hasGivenSheet=0;
  if ((sheetlist = xlsxioread_sheetlist_open(xlsxioread)) != NULL) {
    while ((sheetname = xlsxioread_sheetlist_next(sheetlist)) != NULL) {
      if(!given_sheetname){
        hasGivenSheet=1;
      }else if(strcmp(given_sheetname,sheetname)==0){
        hasGivenSheet=1;
        break;
      }
    }
    xlsxioread_sheetlist_close(sheetlist);
    if(!hasGivenSheet){
      fprintf(stderr, "Given sheet name not found.\n");
      return 1;
    }
  }else{
    fprintf(stderr, "No sheet list found\n");
    return 1;
  }

  //read values from first sheet
  char* value;
  xlsxioreadersheet sheet = xlsxioread_sheet_open(xlsxioread, given_sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS);
  while (xlsxioread_sheet_next_row(sheet)) {
    int $col_id=0;
    while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
      if($col_id)printf(",");
      printf("\"");
      for(i=0;i<strlen(value);i++){
        if(value[i]=='\\' || value[i]=='"'){
          printf("%c",'\\');
        }else{
          printf("%c",value[i]);
        }
      }
      printf("\"");
      $col_id++;
      free(value);
    }
    printf("\n");
  }
  xlsxioread_sheet_close(sheet);

  //clean up
  xlsxioread_close(xlsxioread);
  return 0;
}
