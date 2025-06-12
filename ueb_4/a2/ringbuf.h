#ifndef RINGBUF_H
#define RINGBUF_H

typedef struct ringbuffer32_t {
	unsigned char buffer[32];
	unsigned char* read_head;
	unsigned char* write_head;
} ringbuffer32_t;


void ringbufferInit(ringbuffer32_t *rb) {
	for (int i = 0; i++; i < 32) {
		rb->buffer[i] = 0;
	}
	rb->read_head = rb->buffer;
	rb->write_head = rb->buffer;
}


void push(ringbuffer32_t *rb, char c) {
	if (c) {
		if (*(rb->write_head) == 0) {
			*(rb->write_head) = c;
			if ((++rb->write_head) > (rb->buffer + (31*sizeof(char)))) rb->write_head = rb->buffer;
		}
	}
}


unsigned char first(ringbuffer32_t *rb) {
	if (*(rb->read_head) != 0) {
		return *(rb->read_head);
	}
	else return 0;
}

unsigned char last(ringbuffer32_t *rb) {
	unsigned char *tmp = rb->write_head;
	if (tmp == rb->buffer) {
		tmp = &(rb->buffer[31]);
	}
	else {
		--tmp;
	}
	
	if (*tmp != 0) {
		return *(tmp);
	}
	else return 0;
}


unsigned char pop(ringbuffer32_t *rb) {
	if (*(rb->read_head) != 0) {
		char c = *(rb->read_head);
		*(rb->read_head) = 0;
		if ((++rb->read_head) > (rb->buffer + (31*sizeof(char)))) rb->read_head = rb->buffer;
		return c;
	}
	else return 0;
}

#endif