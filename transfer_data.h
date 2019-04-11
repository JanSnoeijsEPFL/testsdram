void load_param(uint32_t* av_slave, uint32_t* u_ocram, uint32_t* w_ocram, uint32_t* data_ptr);
void ocram_init(uint32_t* u_ocram, uint32_t* w_ocram, uint32_t* x_ocram);
void xocram_fill_RT(uint32_t* x_ocram, uint32_t* data_ptr);
void ocram_empty(uint32_t* ocram, uint32_t RAM_SIZE);

#define UOCRAM_SIZE 307*20
#define WOCRAM_SIZE 3234*20
#define XOCRAM_SIZE 46*20
#define RTDATA_CHUNK_SIZE 20*23
