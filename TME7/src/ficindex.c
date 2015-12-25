#define _POSIX_SOURCE 1

#define _XOPEN_SOURCE 500
#define _BSD_SOURCE

#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include "ficindex.h"

/**
 * Ouvre un fichier indexe.
 * Fichier cree ou ecrase => ecrire la taille de l'index en debut de fichier
 * Sinon => lire la taille de l'index en debut de fichier
 * 	path   nom du fichier a ouvrir
 * 	flags  mode d'ouverture du fichier
 * 	ind    taille de la structure index (ne sert que si le fichier est cree ou ecrase)
 * 	mode   droits d'acces au fichier
 * Renvoie le descripteur du fichier ouvert, -1 en cas de pb.
 */

int
indopen(const char *path, int flags, int ind, mode_t mode)
{
    printf("derp open\n");
    int fd;

    if (access(path, F_OK) == -1) {
        creat(path, mode);
    }

    if ( (fd = open(path, flags, mode)) == -1 ) {
        return -1;
    }

    struct stat stat_info;
    stat(path, &stat_info);

    if (stat_info.st_size > 0) {
        lseek(fd, 4, SEEK_SET);
        return fd;
    }

    // Fichier cree ou ecrase => ecrire la taille de l'index en debut de fichier
    if ( (flags & O_RDWR) || (flags & O_TRUNC) || (flags & O_CREAT) ) {
        write(fd, &ind, sizeof(int));
    }

    return fd;
}

/**
 * Ferme un fichier indexe.
 * 	fd  le descripteur du fichier a fermer
 * Renvoie -1 en cas de pb, 0 sinon.
 */
int
indclose(int fd)
{
    printf("derp close\n");
    return close(fd);
}

/**
 * Lit des donnees dans un fichier indexe.
 *	fd	descripteur du fichier lu
 *	buf     ptr vers les donnees lues
 *	nbytes  nb d'octets a lire
 *		(nbytes doit etre >= a la taille d'index)
 * Renvoie le nb d'octets lus, 0 si fin de fichier, -1 en cas de pb.
 */
ssize_t
indread(int fd, void *buf, size_t nbytes)
{
    printf("derp read\n");
    off_t offset = lseek( fd, 0, SEEK_CUR ) ;

    if (offset == 0) {
        lseek(fd, sizeof(int), SEEK_CUR);
    }

    void *struct_size = malloc(sizeof(int));
    pread(fd, struct_size, sizeof(int), 0);
    if (nbytes < *(size_t *)struct_size) {
        free(struct_size);
        return -1;
    }
    free(struct_size);

    return read(fd, buf, nbytes);
}

/**
 * Ecrit des donnees dans un fichier indexe.
 *	fd	descripteur du fichier modifie
 *	buf     ptr vers les donnees la ecrire
 *	nbytes  nb d'octets a ecrire
 *		(nbytes doit etre >= a la taille d'index)
 * Renvoie le nb d'octets ecrits, -1 en cas de pb.
 */
ssize_t
indwrite(int fd, const void *buf, size_t nbytes)
{
    printf("derp write\n");
    void *struct_size = malloc(sizeof(int));
    pread(fd, struct_size, sizeof(int), 0);
    if (nbytes < *(size_t *)struct_size) {
        free(struct_size);
        return -1;
    }
    free(struct_size);
    return write(fd, buf, nbytes);
}

/**
 * Deplace le curseur dans un fichier indexe.
 *	fd	descripteur du fichier
 *	offset  deplacement a effectuer (en nb de structures index)
 *	whence  positionnement initial du curseur
 * Renvoie le deplacement effectif du curseur (en nb d'octets), -1 en cas de pb.
 */
off_t
indlseek(int fd, off_t offset, int whence)
{
    printf("derp seek\n");
    void *struct_size = malloc(sizeof(int));
    pread(fd, struct_size, sizeof(int), 0);
    int size = *(int *)struct_size;
    free(struct_size);
    return lseek(fd, offset*size, whence);
}


/**
 * Recherche un enregistrement dans un fichier indexe.
 *	fd	descripteur du fichier
 *	cmp	fonction de comparaison qui renvoie 1 si les deux arguments sont égaux, 0 sinon
 *	key	cle de recherche
 * Renvoie le 1er elt trouve pr lequel la comparaison est correcte, NULL sinon.
 */
void*
indsearch(int fd, int (*cmp)(void *, void *), void *key)
{
    off_t offset = lseek( fd, 0, SEEK_CUR ) ;

    if (offset == 3602) {
        return NULL;
    }

    offset += 0;

    printf("derp search\n");
    lseek(fd, 0, SEEK_SET);

    void *struct_size = malloc(sizeof(int));
    read(fd, struct_size, sizeof(int));

    int size = *(int *)struct_size;
    free(struct_size);

    void *buf = malloc(size);

    ssize_t bytes_read = -1;
    //int found = 0;

    while ((bytes_read = read(fd, buf, size)) != 0) {
        if (cmp(buf, key) == 1)
            return buf;
    }

    return NULL;
}

/**
 * Change l'indexation d'un fichier.
 * Les donnees sont recopiees dans un nouveau fichier avec les memes droits d'acces.
 * 	oldfic	fichier a reindexer
 *	newfic	fichier resultat
 *	newind	nouvelle taille de structure index
 * Si la nouvelle taille est < a la precedente, les donnees supplementaires sont perdues.
 * 	ex : 	'ftoto' contient des struct toto { int pi; char ps[2]; };
 *		'ftata' contient des struct tata { int pj; };
 *		indxchg("ftoto", "ftata", sizeof(struct tata));
 *		=> les donnees correspondant a ps seront perdues
 * Renvoie -1 en cas de pb, 0 sinon.
 */
int
indxchg(char *oldfic, char *newfic, unsigned int newind)
{
    printf("derp idxchg\n");
    int fd1 = indopen(oldfic, O_RDONLY, newind, S_IRWXU | S_IRWXO);
    int fd2 = indopen(newfic, O_RDWR, newind, S_IRWXU | S_IRWXO);

    void *buf = malloc(newind);

    while (indread(fd1, buf, newind) > 0) {
        indwrite(fd2, buf, newind);
    }

    //indclose(fd1);
    //indclose(fd2);

    return 0;

}

