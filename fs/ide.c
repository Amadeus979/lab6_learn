/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int dev_addr = 0x13000000;
	int ret = 0;
	int off;
	int mod = 0;
	int status;

	while ( offset + offset_begin < offset_end) {
	    // copy data from source array to disk buffer.
		off = offset + offset_begin;
		
		// disk number write
		ret = syscall_write_dev(&diskno, dev_addr + 0x0010, 4);
		if(ret < 0){
			user_panic("select id wrong");
		}
		// offset write
		ret = syscall_write_dev(&off, dev_addr + 0x0000, 4);
		if(ret < 0){
			user_panic("offset write wrong");
		}
		//start read
		ret = syscall_write_dev(&mod, dev_addr + 0x0020, 4);
		if(ret < 0){
			user_panic("write wrong");
		}
		//get status
		ret = syscall_read_dev(&status, dev_addr + 0x0030, 4);
		if(ret < 0 || status == 0){
			user_panic("offset write wrong");
		}
		// ret = syscall_read_dev((dst + off), dev_addr + 0x4000, 0x200);
		ret = syscall_read_dev((dst + offset), dev_addr + 0x4000, 0x200);
		if(ret < 0){
			user_panic("buffer write wrong");
		}
        //     if error occur, then panic.
		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
	int offset_begin = secno * 0x200; // 0x200 = 512
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int dev_addr = 0x13000000;
	int ret = 0;
	int off;
	int mod = 1;
	int status;

	writef("diskno: %d\n", diskno);
	while ( offset + offset_begin < offset_end) {
	    // copy data from source array to disk buffer.
		off = offset + offset_begin;
		// ret = syscall_write_dev((src + off), dev_addr + 0x4000, 0x200);
		ret = syscall_write_dev((src + offset), dev_addr + 0x4000, 0x200);
		if(ret < 0){
			user_panic("buffer write wrong");
		}
		// disk number write
		ret = syscall_write_dev(&diskno, dev_addr + 0x0010, 4);
		if(ret < 0){
			user_panic("select id wrong");
		}
		// offset write
		ret = syscall_write_dev(&off, dev_addr + 0x0000, 4);
		if(ret < 0){
			user_panic("offset write wrong");
		}
		//start write
		ret = syscall_write_dev(&mod, dev_addr + 0x0020, 4);
		if(ret < 0){
			user_panic("write wrong");
		}
		//get status
		ret = syscall_read_dev(&status, dev_addr + 0x0030, 4);
		if(ret < 0 || status == 0){
			user_panic("offset write wrong");
		}

        //     if error occur, then panic.
		offset += 0x200;
	}
}

