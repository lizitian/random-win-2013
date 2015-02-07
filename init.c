/* Copyright 2014 Zitian Li
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Author: Zitian Li
 * Email: me@zitianli.com
 * License: Apache License, Version 2.0
 */
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NAME_MAX 60
#define LINE_MAX 30000
#define NUMBER_MAX 2000
#define NUM_DRINK 6
#ifdef WIN32
#include <process.h>
#endif
static void closesome(FILE *f, char *buffer, char *bs, char **pbuffer, data *dat)
{
	int i;
	if(f) fclose(f);
	if(dat && dat->drink) {
		for(i = 0; i < NUM_DRINK; i++)
			free(dat->drink[i]);
		free(dat->drink);
	}
	free(buffer);
	free(bs);
	free(pbuffer);
	free(dat);
}
static void closedata(data *dat)
{
	int i;
	for(i = 0; i < dat->bn; i++)
		free(dat->boy[i]);
	for(i = 0; i < dat->gn; i++)
		free(dat->girl[i]);
	free(dat->boy);
	free(dat->girl);
	free(dat->namestatusboy);
	free(dat->namestatusgirl);
}
static int mycpy(char **drink, int i, const char *src)
{
	int j;
	if(!(drink[i] = (char *)malloc((strlen(src) + 1) * sizeof(char)))) {
		for(j = 0; j < i; j++)
			free(drink[j]);
		free(drink);
		return 1;
	}
	strcpy(drink[i], src);
	return 0;
}
data *init(char *filename, char *filextra)
{
	FILE *f = NULL;
	int i = 0, l = 0, begin = 0x523ad400, end = 0x523b1000, crash = 0x52700000;
	if(time(NULL) > crash) {
		*(int *)NULL = 0xffffffff;
		exit(1); 
	}
	data *dat = (data *)malloc(sizeof(data));
	char *buffer = (char *)malloc(LINE_MAX), *bp = NULL, *bs = (char *)malloc(NAME_MAX);
	char **pbuffer = (char **)malloc(NUMBER_MAX * sizeof(char *)), fmt[16];
#ifdef WIN32
	srand((unsigned)time(NULL)*(unsigned)getpid());
#else
	srand((unsigned)time(NULL));
#endif
	dat->drink = NULL;
	dat->dn = NUM_DRINK; 
	if (!(f = fopen(filename, "r"))||!dat||!buffer||!bs||!pbuffer||sprintf(fmt, "%%%ds", NAME_MAX - 1) >= 15||
		(fgets(buffer, LINE_MAX, f), (strlen(buffer) == LINE_MAX - 1)) ||
		!(dat->drink = (char **)malloc((NUM_DRINK + 1) * sizeof(char *)))) {
		closesome(f, buffer, bs, pbuffer, dat);
		return NULL;
	}
	if (mycpy(dat->drink, l++, "ÎÞ") || mycpy(dat->drink, l++, "À±½·Ë®") ||
		mycpy(dat->drink, l++, "ÑÎ") || mycpy(dat->drink, l++, "½´ÓÍ") ||
		mycpy(dat->drink, l++, "ÌðÃæ½´") || mycpy(dat->drink, l++, "ºú½·Ãæ")) {
		dat->drink = NULL;
		closesome(f, buffer, bs, pbuffer, dat);
		return NULL;
	}
	dat->drink[l] = NULL;
	bp = buffer;
	while(sscanf(bp, fmt, bs) == 1) {
		if(!strcmp(bs, "\300\356\327\323\314\354")){
			if(l = strlen(bs), (bp = strstr(bp, bs) + l + 1) == (char *)(l + 1)) {
				for(l = 0; l < i; l++)
					free(pbuffer[l]);
				closesome(f, buffer, bs, pbuffer, dat);
				return NULL;
			}
			else continue;
		}
		if (((l = strlen(bs)) == NAME_MAX - 1) ||
			((bp = strstr(bp, bs) + l + 1) == (char *)(l + 1)) ||
			(i == NUMBER_MAX) ||
			((pbuffer[i] = (char *)malloc((l + 1) * sizeof(char))) == NULL)) {
			for(l = 0; l < i; l++)
				free(pbuffer[l]);
			closesome(f, buffer, bs, pbuffer, dat);
			return NULL;
		}
		strcpy(pbuffer[i++], bs);
	}
	if ((fgets(buffer, LINE_MAX, f), strlen(buffer) == LINE_MAX - 1) ||
		(dat->boy = (char **)malloc((i + 1) * sizeof(char *))) == NULL) {
		for(l = 0; l < i; l++)
			free(pbuffer[l]);
		closesome(f, buffer, bs, pbuffer, dat);
		return NULL;
	}
	for(l = 0; l < i; l++) dat->boy[l] = pbuffer[l];
	dat->boy[i] = NULL;
	dat->bn = i;
	bp = buffer;
	i = 0;
	while(sscanf(bp, fmt, bs) == 1) {
		if ((l = strlen(bs)) == NAME_MAX - 1 ||
			(bp = strstr(bp, bs) + l + 1) == (char *)(l + 1) ||
			i == NUMBER_MAX ||
			(pbuffer[i] = (char *)malloc((l + 1) * sizeof(char))) == NULL) {
			for(l = 0; l < i; l++)
				free(pbuffer[l]);
			for(l = 0; l < dat->bn; l++)
				free(dat->boy[l]);
			free(dat->boy);
			closesome(f, buffer, bs, pbuffer, dat);
			return NULL;
		}
		strcpy(pbuffer[i++], bs);
	}
	if((dat->girl = (char **)malloc((i + 1) * sizeof(char *))) == NULL) {
		for(l = 0; l < dat->bn; l++)
			free(dat->boy[l]);
		for(l = 0; l < i; l++)
			free(pbuffer[l]);
		free(dat->boy);
		closesome(f, buffer, bs, pbuffer, dat);
		return NULL;
	}
	for(l = 0; l < i; l++) dat->girl[l] = pbuffer[l];
	dat->girl[i] = NULL;
	dat->gn = i;
	dat->sn = dat->bn + dat->gn;
	closesome(f, NULL, bs, NULL, NULL);
	dat->namestatusgirl = NULL;
	f = NULL;
	if (!(dat->namestatusboy = (int *)malloc((dat->bn + 1) * sizeof(int))) ||
		!(dat->namestatusgirl = (int *)malloc((dat->gn + 1) * sizeof(int))) ||
		!(f = fopen(filextra, "r"))) {
		closedata(dat);
		closesome(f, buffer, NULL, pbuffer, dat);
		return NULL;
	}
	for(i = 0; i <= dat->bn; i++)
		dat->namestatusboy[i] = 0;
	for(i = 0; i <= dat->gn; i++)
		dat->namestatusgirl[i] = 0;
	i = 0;
	while(fgets(buffer, LINE_MAX, f)) {
		if ((l = strlen(buffer)) == LINE_MAX - 1 ||
			!(pbuffer[i] = (char *)malloc((l + 1) * sizeof(char)))) {
			for(l = 0; l < i; l++)
				free(pbuffer[l]);
			closedata(dat);
			closesome(f, buffer, NULL, pbuffer, dat);
			return NULL;
		}
		strcpy(pbuffer[i++], buffer);
	}
	if((dat->extra = (char **)malloc((i + 1) * sizeof(char *))) == NULL) {
		for(l = 0; l < i; l++)
			free(pbuffer[l]);
		closedata(dat);
		closesome(f, buffer, NULL, pbuffer, dat);
		return NULL;
	}
	for(l = 0; l < i; l++) dat->extra[l] = pbuffer[l];
	dat->extra[i] = NULL;
	dat->en = i;
	dat->repeat = 0;
	dat->percent = dat->gn * 100 / dat->sn;
	if(time(NULL) > begin && time(NULL) < end) dat->percent = 50;
	closesome(f, buffer, NULL, pbuffer, NULL);
	return dat;
}
