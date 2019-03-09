//
//  main.c
//  Product
//
//  Created by Colick on 2018/3/29.
//  Copyright © 2018年 The Big Nerd. All rights reserved.
//

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pwd.h>
#include <unistd.h>

#define SIZE 30000
#define MAX_WORD_SIZE 20
#define MAX_LINE_LETTER 128

typedef struct DictionaryStruct {
    char *word;
    struct DictionaryStruct *next;
} WordLink;

WordLink* dictionary[26][20] = {NULL};

void ConstructDictionary(FILE *file);
void AddToList(WordLink **head, char *string);
char *Trim(char *word);
int Min(int a, int b);
void DealLine(char *line);
void ReadWord(FILE *fp);
char toLowerLetter(char letter);
void SetHomeDirectory(void);

int main() {
    char words[SIZE][24];
    clock_t start, finish;
    start = clock();
    
    //读取英文字典，并返回单词数量
//    SetHomeDirectory();
    FILE *fp;
    fp = fopen("Desktop/words.txt", "r");
    ConstructDictionary(fp);
    fclose(fp);
    
    FILE *fp1;
    fp1 = fopen("Desktop/input.txt", "r");
    ReadWord(fp1);
    fclose(fp1);
    finish = clock();
    printf("Total time:%lf\n", (double)(finish - start) / CLOCKS_PER_SEC);
    
    return 0;
}

void ConstructDictionary(FILE *file) {
    char word[MAX_WORD_SIZE + 1] = {'\0'};
    char *trimWord = NULL;
    while (fgets(word, MAX_WORD_SIZE, file) != NULL) {
        trimWord = Trim(word);
        if (strlen(trimWord) == 0) {
            continue;
        }
        AddToList(&dictionary[toLowerLetter(trimWord[0]) - 'a'][strlen(trimWord)], trimWord);
    }
}

void AddToList(WordLink **head, char *string) {
    char *word = (char *)malloc(strlen(string) + 1);
    strcpy(word, string);
    
    WordLink *item = (WordLink *)malloc(sizeof(WordLink));
    item -> word = word;
    
    item -> next = *head;
    *head = item;
}

char *Trim(char *word) {
    int len = (int)strlen(word);
    int i = 0;
    
    while (!(*word >= 'a' && *word <= 'z') && !(*word >= 'A' && *word <= 'Z'))
        word++;
    
    for (i = len - 1; i > 0 ; i--) {
        if (word[i] < 'a' || word[i] > 'z') {
            word[i] = '\0';
        } else {
            break;
        }
    }
    
    return word;
}

void ReadWord(FILE *fp) {
    char line[MAX_LINE_LETTER + 1] = {'\0'};
    char *trimeLine = NULL;
    while (fgets(line, MAX_LINE_LETTER + 1, fp) != NULL) {
        trimeLine = Trim(line);
        if (strlen(trimeLine) == 0) {
            continue;
        }
        DealLine(Trim(line));
    }
}

void DealLine(char *line) {
    int start = 0;
    int len;
    short isWord = 0;
    char possibleWord[MAX_WORD_SIZE + 1];
    int currentLen = MAX_WORD_SIZE;
    WordLink *head = NULL;
    int oldLen = -1;
    
    while (strlen(line + start) != 0 ){
        isWord = 0;
        len = (int)strlen(line + start);
        if (oldLen == len) {
            if (start == len) {
                break;
            }
            start += 1;
            continue;
        }
        oldLen = len;
        //切分
        strncpy(possibleWord, Trim(line + start), Min(MAX_WORD_SIZE, len));
        currentLen = (int)strlen(possibleWord);//切分的字符串长度
        
        while(!isWord && currentLen != 0) {
            for (head = dictionary[toLowerLetter(possibleWord[0]) - 'a'][currentLen]; head != NULL; head = head -> next) {
                if (strcmp(head -> word, possibleWord) == 0) {
                    start += currentLen;
                    printf("%s ", possibleWord);
                    isWord = 1;
                    break;
                }
            }
            if(!isWord) {
                possibleWord[currentLen - 1] = '\0';
                currentLen--;
            }
        }
        
    }
    putchar('\n');
}

int Min(int a, int b) {
    if (a >= b)
        return b;
    else
        return a;
}

char toLowerLetter(char letter) {
    if (letter >= 'A' && letter <= 'Z') {
        return letter += 32;
    } else {
        return letter;
    }
}

void SetHomeDirectory(void) {
    struct passwd *pw;
    pw = getpwuid(getuid());
    chdir(pw ->pw_dir);
}
