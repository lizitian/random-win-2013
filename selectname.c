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
#include <stdlib.h>
#define myrand(x) (rand() * (x) / (RAND_MAX + 1))
typedef struct {
	int *boy;
	int *girl;
	int bn;
	int gn;
} temp;
static void freetemp(temp *tmp)
{
	free(tmp->boy);
	free(tmp->girl);
	free(tmp);
}
char *selectname(data *dat)
{
	int i, j = 0;
	temp *tmp;
	if(dat->repeat)
		return (myrand(100) < dat->percent) ? (dat->namestatusgirl[i = myrand(dat->gn)]++, dat->girl[i]) :
			(dat->namestatusboy[i = myrand(dat->bn)]++, dat->boy[i]);
	if(!(tmp = (temp *)malloc(sizeof(temp)))) return NULL;
	tmp->girl = NULL;
	tmp->boy = NULL;
	tmp->bn = 0;
	tmp->gn = 0;
	for(i = 0; i < dat->bn; i++)
		if(!dat->namestatusboy[i]) tmp->bn++;
	for(i = 0; i < dat->gn; i++)
		if(!dat->namestatusgirl[i]) tmp->gn++;
	if(tmp->bn == 0) {
		if(tmp->gn == 0 || !(tmp->girl = (int *)malloc((tmp->gn + 1) * sizeof(int)))) {
			free(tmp);
			return NULL;
		}
		for(i = 0; i < dat->gn; i++)
			if(!(dat->namestatusgirl[i])) tmp->girl[j++] = i;
		tmp->girl[j] = 0;
		i = tmp->girl[myrand(tmp->gn)];
		freetemp(tmp);
		return !dat->percent ? NULL : (dat->namestatusgirl[i]++, dat->girl[i]);
	}
	if(tmp->gn == 0) {
		if(!(tmp->boy = (int *)malloc((tmp->bn + 1) * sizeof(int)))) {
			free(tmp);
			return NULL;
		}
		for(i = 0; i < dat->bn; i++)
			if(!(dat->namestatusboy[i])) tmp->boy[j++] = i;
		tmp->boy[j] = 0;
		i = tmp->boy[myrand(tmp->bn)];
		freetemp(tmp);
		return dat->percent == 100 ? NULL : (dat->namestatusboy[i]++, dat->boy[i]);
	}
	if (!(tmp->boy = (int *)malloc((tmp->bn + 1) * sizeof(int))) ||
		!(tmp->girl = (int *)malloc((tmp->gn + 1) * sizeof(int)))) {
		freetemp(tmp);
		return NULL;
	}
	for(i = 0; i < dat->gn; i++)
		if(!(dat->namestatusgirl[i])) tmp->girl[j++] = i;
	tmp->girl[j] = 0;
	j = 0;
	for(i = 0; i < dat->bn; i++)
		if(!(dat->namestatusboy[i])) tmp->boy[j++] = i;
	tmp->boy[j] = 0;
	if(myrand(100) < dat->percent) {
		i = tmp->girl[myrand(tmp->gn)];
		freetemp(tmp);
		return (dat->namestatusgirl[i]++, dat->girl[i]);
	}
	else {
		i = tmp->boy[myrand(tmp->bn)];
		freetemp(tmp);
		return (dat->namestatusboy[i]++, dat->boy[i]);
	}
}
