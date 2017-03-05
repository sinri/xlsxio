#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/xlsxio_read.h"

#ifndef DEBUG_MODE_BY_SINRI
#define DEBUG_MODE_BY_SINRI 0
#endif

void showHelpMessage(){
  printf("%s\n", "SinriExcel2CSV Beta\n------\nUsage: SinriExcel2CSV [-FLAGS] FILE [SHEET]");
  printf("%s\n", "By default, skip empty rows (note: cells may appear empty while they actually contain data)");
  printf("%s\n", "-i : Use ideal mode that skip rows which head column is empty;");
  printf("%s\n", "-c : Skip empty rows and cells;");
  printf("%s\n", "-e : skip extra cells to the right of the rightmost header cell;");
  printf("%s\n", "-n : don't skip any rows or cells;");
  printf("%s\n", "-h : Display This Message.");
}

int main (int argc, char* argv[])
{
  char* filename=NULL;
  char* given_sheetname=NULL;
  int use_ideal_mode=0;
  int i=0;
  char* str=NULL;

  int read_excel_flag=XLSXIOREAD_SKIP_EMPTY_ROWS;

  // SinriExcel2CSV [-FLAGS] FILE [SHEET]
  if(argc==1){
    showHelpMessage();
    return 0;
  }
  for(i=1;i<argc;i++){
    str=argv[i];
    if(strcmp(str,"-h")==0){
      showHelpMessage();
      return 0;
    }
    else if(strcmp(str,"-i")==0){
      //use ideal mode
      use_ideal_mode=1;
    }
    else if(strcmp(str,"-c")==0){
      //use flag (XLSXIOREAD_SKIP_EMPTY_ROWS | XLSXIOREAD_SKIP_EMPTY_CELLS)
      // i.e. XLSXIOREAD_SKIP_ALL_EMPTY
      read_excel_flag=XLSXIOREAD_SKIP_ALL_EMPTY;
    }else if(strcmp(str,"-e")==0){
      //use flag XLSXIOREAD_SKIP_EXTRA_CELLS
      // skip extra cells to the right of the rightmost header cell
      read_excel_flag=XLSXIOREAD_SKIP_EXTRA_CELLS;
    }else if(strcmp(str,"-n")==0){
      read_excel_flag=XLSXIOREAD_SKIP_NONE;
    }
    // if any more flags
    else{
      if(filename==NULL){
        filename=str;
      }else{
        given_sheetname=str;
      }
    }
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
  xlsxioreadersheet sheet = xlsxioread_sheet_open(xlsxioread, given_sheetname, read_excel_flag);
  while (xlsxioread_sheet_next_row(sheet)) {
    int col_id=0;
    int is_empty_row=1;
    while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
      if(use_ideal_mode && col_id==0 && strcmp(value,"")==0){
        break;
      }else{
        is_empty_row=0;
      }
      if(col_id)printf(",");
      printf("\"");
      for(i=0;i<strlen(value);i++){
        if(value[i]=='\\' || value[i]=='"'){
          printf("%c",'\\');
        }
        printf("%c",value[i]);
      }
      printf("\"");
      col_id++;
      free(value);
    }
    if(use_ideal_mode && is_empty_row){
      break;
    }else{
      printf("\n");
    }
  }
  xlsxioread_sheet_close(sheet);

  //clean up
  xlsxioread_close(xlsxioread);
  return 0;
}
