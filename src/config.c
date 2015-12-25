/*
 * Copyright (c) 2015 Mike Maraya <mike[dot]maraya[at]gmail[dot]com>
 * All rights reserved.
 *
 * This file is subject to the terms and conditions defined in
 * https://github.com/mmaraya/port-mirroring/blob/master/LICENSE,
 * which is part of this software package.
 *
 */

#include "config.h"

void find_cfg(struct pm_cfg *cfg) {
    int i;
    if (cfg->flags & PM_DEBUG)
    {
         syslog(LOG_DEBUG, "inside find_cfg()");
    }
    char *path[] = {cfg->cfg_file, CFG_PATH_1, CFG_PATH_2, CFG_PATH_3};
    for (i = 0; i < 4; i++) {
        if (path[i] == NULL)
        {
            break;
        }
        FILE *fp = fopen(path[i], "r");
        if (fp)
        {
            realpath(path[i], cfg->cfg_file);
            printf("set config file to: %s\n", cfg->cfg_file);
            fclose(fp);
            return;
        }
    }
}

char * getUCIItem(char *buf, char *item)
{
    char* p1 = buf;
    char* p2;
    while (*p1 == '\t' || *p1 == ' ') {
        p1++;
    }
    if (*p1 == '\'' || *p1 == '"')
    {
        char delim = *p1++;
        p2    = strchr(p1, delim);
    }
    else
    {
        p2 = strchr(p1, ' ');
        if (p2 == NULL)
        {
            p2 = strchr(p1, '\t');
        }
    }
    if (p2 != NULL)
    {
        *p2 = '\0';
        strncpy(item, p1, strlen(item) - 1);
        item[sizeof(item) - 1] = '\0';
        return p2 + 1;
    }
    else
    {
        return NULL;
    }
}

int getUCIConf(char *buf, char *option, char *value)
{
    char* p = strstr(buf, "option");

    if (p != NULL)
    {
        p += 6;
        if ((p = getUCIItem(p, option)) != NULL)
        {
            if (getUCIItem(p, value) != NULL)
            {
                return 0;
            }
        }
    }
    return -1;
}

