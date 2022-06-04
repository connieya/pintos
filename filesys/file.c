#include "sys/.h"
#include <debug.h>
#include "sys/inode.h"
#include "threads/malloc.h"

/* An open . */
struct  {
	struct inode *inode;        /* 's inode. */
	off_t pos;                  /* Current position. */
	bool deny_write;            /* Has _deny_write() been called? */
};

/* Opens a  for the given INODE, of which it takes ownership,
 * and returns the new .  Returns a null pointer if an
 * allocation fails or if INODE is null. */
struct  *
_open (struct inode *inode) {
	struct  * = calloc (1, sizeof *);
	if (inode != NULL &&  != NULL) {
		->inode = inode;
		->pos = 0;
		->deny_write = false;
		return ;
	} else {
		inode_close (inode);
		free ();
		return NULL;
	}
}

/* Opens and returns a new  for the same inode as .
 * Returns a null pointer if unsuccessful. */
struct  *
_reopen (struct  *) {
	return _open (inode_reopen (->inode));
}

/* Duplicate the  object including attributes and returns a new  for the
 * same inode as . Returns a null pointer if unsuccessful. */
struct  *
_duplicate (struct  *) {
	struct  *n = _open (inode_reopen (->inode));
	if (n) {
		n->pos = ->pos;
		if (->deny_write)
			_deny_write (n);
	}
	return n;
}

/* Closes . */
void
_close (struct  *) {
	if ( != NULL) {
		_allow_write ();
		inode_close (->inode);
		free ();
	}
}

/* Returns the inode encapsulated by . */
struct inode *
_get_inode (struct  *) {
	return ->inode;
}

/* Reads SIZE bytes from  into BUFFER,
 * starting at the 's current position.
 * Returns the number of bytes actually read,
 * which may be less than SIZE if end of  is reached.
 * Advances 's position by the number of bytes read. */
off_t
_read (struct  *, void *buffer, off_t size) {
	off_t bytes_read = inode_read_at (->inode, buffer, size, ->pos);
	->pos += bytes_read;
	return bytes_read;
}

/* Reads SIZE bytes from  into BUFFER,
 * starting at offset _OFS in the .
 * Returns the number of bytes actually read,
 * which may be less than SIZE if end of  is reached.
 * The 's current position is unaffected. */
off_t
_read_at (struct  *, void *buffer, off_t size, off_t _ofs) {
	return inode_read_at (->inode, buffer, size, _ofs);
}

/* Writes SIZE bytes from BUFFER into ,
 * starting at the 's current position.
 * Returns the number of bytes actually written,
 * which may be less than SIZE if end of  is reached.
 * (Normally we'd grow the  in that case, but  growth is
 * not yet implemented.)
 * Advances 's position by the number of bytes read. */
off_t
_write (struct  *, const void *buffer, off_t size) {
	off_t bytes_written = inode_write_at (->inode, buffer, size, ->pos);
	->pos += bytes_written;
	return bytes_written;
}

/* Writes SIZE bytes from BUFFER into ,
 * starting at offset _OFS in the .
 * Returns the number of bytes actually written,
 * which may be less than SIZE if end of  is reached.
 * (Normally we'd grow the  in that case, but  growth is
 * not yet implemented.)
 * The 's current position is unaffected. */
off_t
_write_at (struct  *, const void *buffer, off_t size,
		off_t _ofs) {
	return inode_write_at (->inode, buffer, size, _ofs);
}

/* Prevents write operations on 's underlying inode
 * until _allow_write() is called or  is closed. */
void
_deny_write (struct  *) {
	ASSERT ( != NULL);
	if (!->deny_write) {
		->deny_write = true;
		inode_deny_write (->inode);
	}
}

/* Re-enables write operations on 's underlying inode.
 * (Writes might still be denied by some other  that has the
 * same inode open.) */
void
_allow_write (struct  *) {
	ASSERT ( != NULL);
	if (->deny_write) {
		->deny_write = false;
		inode_allow_write (->inode);
	}
}

/* Returns the size of  in bytes. */
off_t
_length (struct  *) {
	ASSERT ( != NULL);
	return inode_length (->inode);
}

/* Sets the current position in  to NEW_POS bytes from the
 * start of the . */
void
_seek (struct  *, off_t new_pos) {
	ASSERT ( != NULL);
	ASSERT (new_pos >= 0);
	->pos = new_pos;
}

/* Returns the current position in  as a byte offset from the
 * start of the . */
off_t
_tell (struct  *) {
	ASSERT ( != NULL);
	return ->pos;
}
