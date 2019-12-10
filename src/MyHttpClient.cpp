/*
 * MyHttpClient.cpp
 *
 *  Created on: Dec 9, 2019
 *      Author: root
 */

#include "MyHttpClient.h"

MyHttpClient::MyHttpClient() {
    // TODO Auto-generated constructor stub

}

MyHttpClient::~MyHttpClient() {
    // TODO Auto-generated destructor stub
}

void MyHttpClient::client_handler(struct mg_connection *nc, int ev, void *ev_data)
{
    struct http_message *hm = (struct http_message *) ev_data;

    switch (ev)
    {
        case MG_EV_CONNECT:
        {
            if (*(int *) ev_data != 0)
            {
                fprintf(stderr, "connect() failed: %s\n", strerror(*(int *) ev_data));
                s_exit_flag = 1;
            }
            break;
        }

        case MG_EV_HTTP_REPLY:
        {
            nc->flags |= MG_F_CLOSE_IMMEDIATELY;
            if (s_show_headers)
            {
                fwrite(hm->message.p, 1, hm->message.len, stdout);
            }
            else
            {
                fwrite(hm->body.p, 1, hm->body.len, stdout);
            }
            putchar('\n');
            s_exit_flag = 1;
            break;
        }

        case MG_EV_CLOSE:
        {
            if (s_exit_flag == 0)
            {
                printf("Server closed connection\n");
                s_exit_flag = 1;
            }
            break;
        }
        default:
            break;
    }
}

int MyHttpClient::testHttpClient()
{
    struct mg_mgr mgr;
    int i;
    char url[256] = "www.baidu.com";
    mg_mgr_init(&mgr, NULL);

    mg_connect_http(&mgr, client_handler, url, NULL, NULL);

    while (s_exit_flag == 0) {
      mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}
