#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

enum
{
	NAME,
	TYPE,
	EXEC,
	EMPTY,
	NEWER,
	INVALID,
};
typedef  struct
{
	int type;
	union
	{
		const char *motif;
		const char *chemin;
		char type_fichier;
	} operande;
} expression;

int type_correspond(const struct stat *buf, char type){
        switch(buf->st_mode & S_IFMT){
                case S_IFSOCK:
                        return type == 's';
                case S_IFLNK:
                        return type == 'l';
                case S_IFREG:
                        return type == 'f';
                case S_IFBLK:
                        return type == 'b';
                case S_IFDIR:
                        return type == 'd';
                case S_IFCHR:
                        return type == 'c';
                case S_IFIFO:
                        return type == 'p';
                default:
                        return 0;
        }
}

int executable(const struct stat *buf){
        return (buf->st_mode & S_IRWXU) == S_IXUSR || (buf->st_mode & S_IRWXG) == S_IXGRP || (buf->st_mode & S_IRWXO) == S_IXOTH;
}
int nom_correspond(const char *chemin, const char *motif){
        return strcmp(strrchr(chemin,'/')+1,motif) == 0;
}

int vide(const struct stat *buf){
        return  buf->st_size == 0;
}
int plus_recent(const struct stat *buf, const char *fichier_reference){
        struct stat buf2;
        stat(fichier_reference, &buf2);
        return buf2.st_mtim.tv_nsec < buf->st_mtim.tv_nsec;
}

void traiter_fichier(const char* chemin, const expression *exp, int suivre_lien){
	struct stat file;
	suivre_lien == 1 ? lstat(chemin, &file) : stat(chemin, &file);
	switch(exp->type){
		case NAME:
			if(nom_correspond(chemin,exp->operande.motif))
				printf("%s\n", chemin);
		break;
		case TYPE:
			if(type_correspond(&file, exp->operande.type_fichier))
				printf("%s\n", chemin);
		break;
		case EXEC:
			if(executable(&file))
				printf("%s\n", chemin);
		break;
		case EMPTY:
			if(vide(&file))
				printf("%s\n", chemin);
		break;
		case NEWER:
			if(plus_recent(&file, chemin))
				printf("%s\n", chemin);		
		break;
	}		
}

void parcourir_repertoire(const char* chemin, const expression *exp, int suivre_lien){
	DIR *dir = opendir(chemin);
	struct dirent *entry = readdir(dir);
	while(entry != NULL){
		char buff[strlen(chemin)+strlen(entry->d_name)+2];
                sprintf(buff,"%s/%s",chemin,entry->d_name);
		if(entry->d_type == DT_DIR && entry->d_name[0] != '.')
			parcourir_repertoire(buff, exp, suivre_lien);
		else
			traiter_fichier(buff, exp, suivre_lien);
		entry = readdir(dir);
	}
	closedir(dir);
	
}

void erreur(){
	printf("Usage : find [-L|-P] [path] [expression]\nexpression :\n-name <name>\n-type <type> (f,d,l,b,c,p,s)\n-executable\n-empty\n-anewer <file>\n");
	exit(1);
}


int main(int argc, char **argv){
	if(argc < 2) // ERROR
		return 1;

	int linkFlag = 0;
	int i = 1;
	char *path;
	expression exp;	


	if(argv[1][0] == '-'){
		if(argv[1][1] == 'L')
			linkFlag = 1;
		else if (argv[1][1] != 'P')
			erreur();
		i++;
	}
	path = argv[i];
	i++;
	while(i < argc-1){
		if(argv[i][0] == '-'){
			if(strcmp(argv[i]+1, "name")){
				exp.type = NAME;
			} else if(strcmp(argv[i]+1, "type"i){                             
                         	exp.type = TYPE;
                        } else if(strcmp(argv[i]+1, "executable")){                             
                         	exp.type = exp.EXEC;
                        } else if(strcmp(argv[i]+1, "empty")){                             
                         	exp.type = EMPTY;
                        } else if(strcmp(argv[i]+1, "anewer")){                             
                         	exp.type = NEWER;
			} else {
				erreur();
			}			
		} else if (exp.type != NULL){
			//Handle arguments
			switch(exp.type){
                		case NAME:
					exp.operande.motif = argv[i];
         		        break;
                		case TYPE:
					exp.operande.type_fichier = argv[i][0];
              			break;
              			case NEWER:
                      			exp.operande.chemin = argv[i];
              		  	break;
				default:
					erreur();
				break;
       			}
		} else {
			erreur();
		}
		i++;
	
	parcourir_repertoire(path, &exp, linkFlag);

	return 0;
}
