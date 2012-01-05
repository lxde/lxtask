/* $Id: xfce-taskmanager-linux.c 3940 2008-02-10 22:48:45Z nebulon $
 *
 * Copyright (c) 2006 Johannes Zellner, <webmaster@nebulon.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include "xfce-taskmanager-linux.h"


void get_task_details(gint pid,struct task *task)
{
	int fd;
	gchar line[256];

	task->pid=-1;
	task->checked=FALSE;
        task->size=0;

	sprintf(line,"/proc/%d/statm",pid);
        fd=open(line,O_RDONLY);
        if(fd==-1) return;
        read(fd,line,256);
        sscanf(line,"%d %d",&task->size,&task->rss);
        close(fd);
        if(!task->size) return;
	task->size*=PAGE_SIZE;
	task->rss*=PAGE_SIZE;

	sprintf(line,"/proc/%d/stat",pid);
	fd=open(line,O_RDONLY);
	if(fd!=-1)
	{
		struct passwd *passwdp;
		struct stat st;
		char buf[2048],*p=buf;
		size_t len;
		gint utime = 0;
		gint stime = 0;

		task->pid=pid;
		
		read(fd,buf,2048);
		p=strchr(buf,'(');p++;
		for(len=0;*p!=')';len++) task->name[len]=*p++;
		task->name[len]=0;p++;
		if(show_full_path)
		{
			FILE *fp;
			sprintf(line,"/proc/%d/cmdline",pid);
			fp=fopen(line,"r");
			if(fp)
			{
				size_t size;
				size=fread(task->name,1,sizeof(task->name)-1,fp);
				if(size>0)
				{
					int i;
					task->name[size]=0;
					for(i=0;i<size;i++)
					{
						if(task->name[i]=='\0')
						{
							if(task->name[i+1]=='\n')
								break;
							task->name[i]=' ';
						}
					}
				}
				fclose(fp);
			}
		}
		else if(len>=15)
		{
			FILE *fp;
			sprintf(line,"/proc/%d/cmdline",pid);
			fp=fopen(line,"r");
			if(fp)
			{
				char *p;
				fscanf(fp, "%255s", line);
				fclose(fp);
				p=strrchr(line,'/');
				if(p != NULL)
					strcpy(task->name, p+1);
				else
					strcpy(task->name, line);
			}
		}

		sscanf(p, "%1s %i %s %s %s %s %s %s %s %s %s %i %i %s %s %s %i",
                        task->state, // processstate
                        &task->ppid, // parentid
                        line,      // processs groupid

                        line,      // session id
                        line,      // tty id
                        line,      // tpgid: The process group ID of the process running on tty of the process
                        line,      // flags
                        line,      // minflt minor faults the process has maid

                        line,      // cminflt
                        line,      // majflt
                        line,      // cmajflt
                        &utime,     // utime the number of jiffies that this process has scheduled in user mode
                        &stime,     // stime " kernel mode

                        line,      // cutime " waited for children in user
                        line,      // cstime " kernel mode
                        line,      // priority (nice value + fifteen)
                        &task->prio // nice range from 19 to -19    /* my change */
                    );
		task->time = stime + utime;
		task->old_time = task->time;
		task->time_percentage = 0;

		fstat(fd,&st);
		task->uid=st.st_uid;
		passwdp = getpwuid(task->uid);
		if( passwdp != NULL && passwdp->pw_name != NULL)
			g_strlcpy(task->uname, passwdp->pw_name, sizeof task->uname);

		close(fd);
	}
}

static int proc_filter(const struct dirent *d)
{
    int c=d->d_name[0];
    return c>='1' && c<='9';
}

GArray *get_task_list(void)
{
    GArray *task_list;
    struct dirent **namelist;
    int n;
    int count=0;


    task_list = g_array_new(FALSE, FALSE, sizeof (struct task));
    n=scandir("/proc",&namelist,proc_filter,0);
    if(n<0) return task_list;

    g_array_set_size(task_list,n);
    while(n--)
    {
        int pid=atoi(namelist[n]->d_name);
        struct task *task=&g_array_index(task_list, struct task, count);
	free(namelist[n]);
        get_task_details(pid,task);
        if(task->pid != -1 && task->size>0)	// don't show error or kenerl threads
        {
            count++;
        }
    }
    task_list->len=count;
    free(namelist);
    return task_list;
}

gboolean get_cpu_usage_from_proc(system_status *sys_stat)
{
    const gchar *file_name = "/proc/stat";
    FILE *file;

    if ( sys_stat->valid_proc_reading == TRUE ) {
        sys_stat->cpu_old_jiffies =
            sys_stat->cpu_user +
            sys_stat->cpu_nice +
            sys_stat->cpu_system+
            sys_stat->cpu_idle;
        sys_stat->cpu_old_used =
            sys_stat->cpu_user +
            sys_stat->cpu_nice +
            sys_stat->cpu_system;
    } else {
        sys_stat->cpu_old_jiffies = 0;
    }

    sys_stat->valid_proc_reading = FALSE;

    file = fopen (file_name, "rb");
    if(!file) return FALSE;
    if ( fscanf (file, "cpu\t%u %u %u %u",
                 &sys_stat->cpu_user,
                 &sys_stat->cpu_nice,
                 &sys_stat->cpu_system,
                 &sys_stat->cpu_idle
                ) == 4 )
    {
        sys_stat->valid_proc_reading = TRUE;
    }
    fclose( file );
    return TRUE;
}

gboolean get_system_status (system_status *sys_stat)
{
    FILE *file;
    gchar buffer[100];
    int reach;
    static int cpu_count;

    file = fopen ("/proc/meminfo", "r");
    if(!file) return FALSE;
    reach=0;
    while (fgets (buffer, 100, file) != NULL)
    {
        if(!strncmp(buffer,"MemTotal:",9))
            sys_stat->mem_total=atoi(buffer+10),reach++;
        else if(!strncmp(buffer,"MemFree:",8))
            sys_stat->mem_free=atoi(buffer+9),reach++;
        else if(!strncmp(buffer,"Cached:",7))
            sys_stat->mem_cached=atoi(buffer+8),reach++;
        else if(!strncmp(buffer,"Buffers:",8))
            sys_stat->mem_buffered=atoi(buffer+9),reach++;
        if(reach==4) break;
    }
    fclose (file);

    if(!cpu_count)
    {
        file = fopen ("/proc/cpuinfo", "r");
        if(!file) return FALSE;
        while (fgets (buffer, 100, file) != NULL)
        {
            if(buffer[0]!='p') continue;
            if(!strncmp(buffer,"processor",9))
            {
                cpu_count++;
            }
        }
        fclose (file);
    }
    sys_stat->cpu_count=cpu_count;
    return TRUE;
}

void send_signal_to_task(gint task_id, gint signal)
{
    if(task_id > 0 && signal != 0)
    {
        gint ret = 0;

        ret = kill(task_id, signal);

        if(ret != 0)
            show_error(_("Couldn't send signal %d to the task with ID %d\n\n%s"), signal, task_id, g_strerror(errno) );
    }
}


void set_priority_to_task(gint task_id, gint prio)
{
    if(task_id > 0)
    {
    	int status = 0;
        gchar command[128] = "";
        g_sprintf(command, "renice %d %d > /dev/null", prio, task_id);

        if( ! g_spawn_command_line_sync(command, NULL, NULL, &status, NULL) ||status )
            show_error(_("Couldn't set priority %d to the task with ID %d"), prio, task_id);
    }
}

