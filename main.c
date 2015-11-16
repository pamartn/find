#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

void traiter_fichier(const char* chemin, const expression *exp, int suivre_lien){
	struct stat file;
	suivre_lien == 1 ? lstat(chemin, file) : stat(chemin, file);
	if(exp->type){
		case NAME:
			if(nom_correspond(chemin,exp->operande.motif))
				printf("%s\n", chemin);
		break;
		case TYPE:
			if(type_correspond(file, exp->operande.type_fichier))
				printf("%s\n", chemin);
		break;
		case EXEC | EMPTY:
			if(executable(buf) | vide(buf))
				printf("%s\n", chemin);
		break;
		case NEWER:
				
		break;
	}		
}

int nom_correspond(const char *chemin, const char *motif){
	return strcmp(strrchr(chemin,'/')+1,motif) == 0;
}

void parcourir_repertoire(const char* chemin){
	DIR *dir = opendir(chemin);
	struct dirent *entry = readdir(dir);
	while(entry != NULL){
		char buff[strlen(chemin)+strlen(entry->d_name)+2];
                sprintf(buff,"%s/%s",chemin,entry->d_name);
		if(entry->d_type == DT_DIR)
			parcourir_repertoire(buff);
		else
			traiter_fichier(buff);
		entry = readdir(dir);
	}
	
}

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

int vide(const struct stat *buf){
	return  buf->st_size == 0;
}

int plus_recent(const struct stat *buf, const char *fichier_reference){
		
}

int main(int argc, char **argv){
	if(argc < 2)
		return 1;
	parcourir_repertoire(argv[1]);
	
	int i = nom_correspond("coucou/test","test");
	printf("%d\n",i);
	return 0;
}
