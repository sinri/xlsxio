#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/xlsxio_write.h"

/**
 * Use this to convert file from utf-8 to gbk
 * iconv -f UTF-8 -t GBK -c test.csv  > test_gbk.csv
 * UTF-8 : read by this and Mac Sublime
 * GBK : read by Excel in Mac or Windows
 */

#ifndef GRAY_BGCOLOR_START
#define GRAY_BGCOLOR_START "\033[47m"
#endif

#ifndef GREEN_BGCOLOR_START
#define GREEN_BGCOLOR_START "\033[42m"
#endif

#ifndef BGCOLOR_END
#define BGCOLOR_END "\033[0m"
#endif

struct node {
    char* c;
    struct node *next;
};

void debug(const char*string,int line_no){
    printf("%s[DEBUG]<%d> %s%s\n",GRAY_BGCOLOR_START,line_no, string,BGCOLOR_END);
}

struct node* tailNode(struct node* base){
    struct node* p=base;
    while(p->next){
        p=p->next;
    }
    return p;
}

struct node* appendToNode(struct node* base,const char* c){
    if(base){
        struct node* old_tail=tailNode(base);
        struct node* tail=(struct node*)malloc(sizeof(struct node));
        tail->c=(char*)malloc(sizeof(char)*(strlen(c)));
        strcpy(tail->c,c);
        tail->next=NULL;
        old_tail->next=tail;
    }else{
        base=(struct node*)malloc(sizeof(struct node));
        base->c=(char*)malloc(sizeof(char)*(strlen(c)));
        strcpy(base->c,c);
        base->next=NULL;
    }
    return base;
}

void writeRow(xlsxiowriter handle,struct node *row){
    static int rowIndex=0;
    //TODO
    if(rowIndex==0){
        //set row height
        xlsxiowrite_set_row_height(handle, 1);
        //how many rows to buffer to detect column widths
        xlsxiowrite_set_detection_rows(handle, 10);

        struct node *cell = row;
        while(cell){
            // xlsxiowrite_add_column(handle, cell->c, 0);//title row, no use better kamo?
            xlsxiowrite_add_cell_string(handle, cell->c);
            cell=cell->next;
        }
        xlsxiowrite_next_row(handle);
    }else{
        struct node *cell = row;
        while(cell){
            xlsxiowrite_add_cell_string(handle, cell->c);
            cell=cell->next;
        }
        xlsxiowrite_next_row(handle);
    }
    //OVER
    rowIndex++;
}

int main (int argc, char* argv[])
{
    FILE* src;
    char* csv_filename;
    char* xlsx_filename;

    if(argc>1){
        csv_filename=argv[1];
        if(argc>2){
            xlsx_filename=argv[2];
        }else{
            if ((xlsx_filename = (char*)malloc(strlen(csv_filename) + 6)) == NULL ){
                fprintf(stderr, "Memory allocation error\n");
                return 1;
            } else {
                strcpy(xlsx_filename, csv_filename);
                strcat(xlsx_filename, ".xlsx");
            }
        }
    }
    else{
        printf("%s\n", "parameters lack");
        return 1;
    }

    if ((src = fopen(csv_filename, "rb")) == NULL) {
      fprintf(stderr, "Error opening file: %s\n", csv_filename);
      return 1;
    }

    xlsxiowriter handle;
    //open .xlsx file for writing (will overwrite if it already exists)
    if ((handle = xlsxiowrite_open(xlsx_filename, "MySheet")) == NULL) {
        fprintf(stderr, "Error creating .xlsx file\n");
        return 1;
    }

    //skip UTF-8 BOM header if present
    unsigned char bom[3];
    if (!(fread(bom, 1, 3, src) == 3 && bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)) {
      fseek(src, 0, SEEK_SET);
    }
    //read
    char oneBuffer[1];
    int char_status=0;// [0]"[1] X[1] \[2]\[1] \[2]"[1] "[0],[0]X[3],[0],\[4]\[3]
    char cellBuffer[1024]={0};// dymatic better?
    int cellBufferPtr=0;
    struct node * row=NULL;
    while(fread(oneBuffer,sizeof( char ), 1,src)){
        switch(char_status){
            case 0:{
                if(oneBuffer[0]=='"'){
                    // begin cellBuffer, quote begin
                    cellBufferPtr=0;
                    cellBuffer[cellBufferPtr]=0;
                    char_status=1;
                }
                else if(oneBuffer[0]==','){
                    // one cell buffer ended
                    row=appendToNode(row,cellBuffer);
                    cellBufferPtr=0;
                    cellBuffer[cellBufferPtr]=0;
                    char_status=0;
                }
                else if(oneBuffer[0]=='\n'){
                    //row over
                    if(cellBuffer[0]){
                        row=appendToNode(row,cellBuffer);
                        cellBufferPtr=0;
                        cellBuffer[cellBufferPtr]=0;
                        // write
                        writeRow(handle,row);
                        row=NULL;
                        char_status=0;
                    }
                }
                else{
                    //standard input chat
                    cellBuffer[cellBufferPtr++]=oneBuffer[0];
                    cellBuffer[cellBufferPtr]=0;
                }
            }
            break;
            case 1:{
                if(oneBuffer[0]=='\\'){
                    //escape found
                    char_status=2;
                }
                else if(oneBuffer[0]=='"'){
                    //quote over
                    char_status=0;
                }else{
                    //standard input chat
                    cellBuffer[cellBufferPtr++]=oneBuffer[0];
                    cellBuffer[cellBufferPtr]=0;
                }
            }
            break;
            case 2:{
                if(oneBuffer[0]=='n'){
                    cellBuffer[cellBufferPtr++]='\n';
                    cellBuffer[cellBufferPtr]=0;
                }
                else if(oneBuffer[0]=='"'){
                    cellBuffer[cellBufferPtr++]='"';
                    cellBuffer[cellBufferPtr]=0;
                }
                else if(oneBuffer[0]=='t'){
                    cellBuffer[cellBufferPtr++]='\t';
                    cellBuffer[cellBufferPtr]=0;
                }
                else if(oneBuffer[0]=='\\'){
                    cellBuffer[cellBufferPtr++]='\\';
                    cellBuffer[cellBufferPtr]=0;
                }
                else {
                    cellBuffer[cellBufferPtr++]='\\';
                    cellBuffer[cellBufferPtr++]=oneBuffer[0];
                    cellBuffer[cellBufferPtr]=0;
                }
                char_status=1;
            }
            break;
            case 3:{
                if(oneBuffer[0]=='\\'){
                    char_status=4;
                }
                else if (oneBuffer[0]==','){
                    // cell over
                    row=appendToNode(row,cellBuffer);
                    cellBufferPtr=0;
                    cellBuffer[cellBufferPtr]=0;
                }
                else if(oneBuffer[0]=='\n'){
                    // cell over
                    row=appendToNode(row,cellBuffer);
                    cellBufferPtr=0;
                    cellBuffer[cellBufferPtr]=0;
                    // write
                    writeRow(handle,row);
                    row=NULL;
                    char_status=0;
                }
                else{
                    cellBuffer[cellBufferPtr++]=oneBuffer[0];
                    cellBuffer[cellBufferPtr]=0;
                }
            }
            break;
            case 4: {
                if(oneBuffer[0]=='\n'){
                    cellBuffer[cellBufferPtr++]='\\';
                    cellBuffer[cellBufferPtr++]=oneBuffer[0];
                    cellBuffer[cellBufferPtr]=0;
                    // cell over
                    row=appendToNode(row,cellBuffer);
                    cellBufferPtr=0;
                    cellBuffer[cellBufferPtr]=0;
                    // write
                    writeRow(handle,row);
                    row=NULL;
                    char_status=0;
                }else{
                    if(oneBuffer[0]=='n'){
                        cellBuffer[cellBufferPtr++]='\n';
                        cellBuffer[cellBufferPtr]=0;
                    }
                    else if(oneBuffer[0]=='"'){
                        cellBuffer[cellBufferPtr++]='"';
                        cellBuffer[cellBufferPtr]=0;
                    }
                    else if(oneBuffer[0]=='t'){
                        cellBuffer[cellBufferPtr++]='\t';
                        cellBuffer[cellBufferPtr]=0;
                    }
                    else if(oneBuffer[0]=='\\'){
                        cellBuffer[cellBufferPtr++]='\\';
                        cellBuffer[cellBufferPtr]=0;
                    }
                    else {
                        cellBuffer[cellBufferPtr++]='\\';
                        cellBuffer[cellBufferPtr++]=oneBuffer[0];
                        cellBuffer[cellBufferPtr]=0;
                    }
                    char_status=3;
                }
            }
            default:
            {
                fprintf(stderr, "Error reading .csv file:\nchar_status=%d, read char=%c\n",char_status,oneBuffer[0]);
                return 1;
            }
            break;
        }
    }
    fclose(src);

    //close .xlsx file
    xlsxiowrite_close(handle);

    return 0;
}