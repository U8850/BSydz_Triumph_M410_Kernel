#ifndef __GDM_SDIO_H__
#define __GDM_SDIO_H__

#include <linux/types.h>
#include <linux/time.h>

#define MAX_NR_SDU_BUF  64

struct sdio_tx {
	struct list_head	list;
	struct tx_cxt		*tx_cxt;

	u8	*buf;
	int	len;

	void (*callback)(void *cb_data);
	void *cb_data;
};

struct tx_cxt {
	struct list_head	free_list;
	struct list_head	sdu_list;
	struct list_head	hci_list;
	struct timeval		sdu_stamp;

	u8	*sdu_buf;

	spinlock_t			lock;
	int	can_send;
	int stop_sdu_tx;
};

struct sdio_rx {
	struct list_head	list;
	struct rx_cxt		*rx_cxt;

	void (*callback)(void *cb_data, void *data, int len);
	void *cb_data;
};

struct rx_cxt {
	struct list_head	free_list;
	struct list_head	req_list;

	u8		*rx_buf;
	int		rx_len;
	int		pos;

	spinlock_t			lock;
};

struct sdiowm_dev {
	struct sdio_func	*func;
	
	struct tx_cxt	tx;
	struct rx_cxt	rx;

	struct work_struct	ws;
	struct list_head	pm_list;
	u8	suspended;	
};

#endif // __GDM_SDIO_H__
