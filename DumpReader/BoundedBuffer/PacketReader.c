/*
 * Implementation of a packetreader
 */

#include <stdlib.h>
#include <stdio.h>
#include "../DumpReader.h"
#include "PacketReader.h"

//#define HEADER_SIZE 16
//Shimrit - my change
#define HEADER_SIZE 0

unsigned int readUIntValue(uchar *buff, int idx) {
	unsigned int res;
	int i;
	res = 0;
	for (i = 0; i < 4; i++) {
		res <<= 8;
		res |= buff[idx + i] & 0x0FF;
	}
	return res;
}

void printUIntValue(uchar *buff, int idx, FILE *stream) {
	int i;
	for (i = 0; i < 4; i++) {
		fprintf(stream, "%02X", buff[idx + i] & 0x0FF);
	}
}

//Shimrit - my changes
void *start_packetreader_thread(void *param) {
	PacketReaderData *packetreader;
			Packet *p;
			FILE *f;

			char *data;
			size_t len;
			ssize_t read;
			char * line = NULL;
			int count, q;
			LinkedList *curr_queue;

			count = 0;
			q = 0;

			packetreader = (PacketReaderData*)param;

			f = fopen(packetreader->path, "rb");
			if (!f) {
				fprintf(stderr, "Cannot read file %s\n", packetreader->path);
				exit(1);
			}
			while ((read = getline(&line, &len, f)) != -1) {
				p = (Packet*)malloc(sizeof(Packet));

				//Shimrit - my changes
				p->size = read;
				p->source=0;
				p->dest =0;
				p->time=0;

				if (p->size <= 0 || p->size > 100000000) {
					fprintf(stderr, "Invalid packet size: %u\n", p->size);
					fprintf(stderr, "\t(Size bytes were:");
					fprintf(stderr, ")\n");
					exit(1);
				}

				data = (char*)malloc(sizeof(char) * (p->size + 1));
				strncpy(data,line,read);
				data[p->size] = '\0';

				p->data = data;

				curr_queue = packetreader->packet_queues[q];
				q = (q + 1) % (packetreader->num_queues);

				list_enqueue(curr_queue, p);
				count++;

				packetreader->size += p->size;
				packetreader->sizeWithHeaders += p->size + HEADER_SIZE;
			}

			fclose(f);
			for (q = 0; q < packetreader->num_queues; q++) {
				list_set_done(packetreader->packet_queues[q]);
			}

			return NULL;

}

void packetreader_init(PacketReaderData *packetreader, const char *path, LinkedList *packet_queues, int num_queues) {
	int i;
	packetreader->done = 0;
	packetreader->path = path;
	packetreader->size = 0;
	packetreader->sizeWithHeaders = 0;

	for (i = 0; i < num_queues; i++) {
		packetreader->packet_queues[i] = &(packet_queues[i]);
	}
	for (; i < MAX_QUEUES; i++) {
		packetreader->packet_queues[i] = NULL;
	}

	packetreader->num_queues = num_queues;
}

int packetreader_start(PacketReaderData *packetreader) {
	return pthread_create(&(packetreader->thread_id), NULL, start_packetreader_thread, (void*)packetreader);
}

void packetreader_join(PacketReaderData *packetreader) {
	pthread_join(packetreader->thread_id, NULL);
}
