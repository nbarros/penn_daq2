#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

#include "NetUtils.h"

struct event_base *evBase;
struct evconnlistener *contListener, *viewListener, *xl3Listener[MAX_XL3_CON];

int setupListeners()
{
  evBase = event_base_new();
  const char **methods = event_get_supported_methods();
  printf("Starting Libevent %s. Supported methods are:\n",
      event_get_version());
  for (int i=0;methods[i] != NULL; i++){
    printf("\t%s\n",methods[i]);
  }
  free((char**)methods);
  printf("Using %s.\n",event_base_get_method(evBase));

  struct event *signalEvent;
  signalEvent = evsignal_new(evBase, SIGINT, signalCallback, (void*) evBase);
  if (!signalEvent || event_add(signalEvent, NULL) < 0){
    printf("Could not create / add a signal event!\n");
    return -1;
  }
  printf("done\n");

  return 0;
}

void signalCallback(evutil_socket_t sig, short events, void *user_data)
{
  printf("\nCaught an interrupt signal, exiting.\n");
  exit(1);
}