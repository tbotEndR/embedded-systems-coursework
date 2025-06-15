#ifndef RINGBUF_H
#define RINGBUF_H
#define RINGBUFFERSIZE 32

typedef struct ringbuffer32_t {
	unsigned char buffer[RINGBUFFERSIZE];
	unsigned char* read_head;
	unsigned char* write_head;
} ringbuffer32_t;


void ringbufferInit(ringbuffer32_t *rb) {
	rb->read_head = rb->buffer;
	rb->write_head = rb->buffer;
}


/*	writes c to the current write_head position. won't overwrite not yet read cells (increment over read_head) */
void push(ringbuffer32_t *rb, char c) {
	if (c) {
		*(rb->write_head) = c;
		if (rb->write_head != &rb->buffer[RINGBUFFERSIZE-1]) {
			if (rb->read_head != (rb->write_head + 1)) rb->write_head++;
		} else if (rb->read_head != rb->buffer) rb->write_head = rb->buffer;
	}
}


/*	writes c to the current write_head position. will not increment read_head past write_head */
unsigned char pop(ringbuffer32_t *rb) {
	if (rb->read_head == rb->write_head) return 0;
	char c = *(rb->read_head);
	if (rb->read_head != rb->write_head)
		if (++rb->read_head > &rb->buffer[RINGBUFFERSIZE-1]) rb->read_head = rb->buffer;
	return c;
}

unsigned char head(ringbuffer32_t *rb) {
	if (rb->read_head != rb->write_head) {
		return *(rb->read_head);
	}
	else return 0;
}

unsigned char tail(ringbuffer32_t *rb) {
	unsigned char *tmp = rb->write_head;
	if (tmp == rb->buffer) tmp = &(rb->buffer[RINGBUFFERSIZE-1]);
	else --tmp;
	return *tmp;
}

#endif