#include <iostream>
#include <libssh2.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#endif
namespace socketpp {
class Network_init_handle {
public:
  Network_init_handle() { init(); }
  ~Network_init_handle() { deinit(); }
  static void init() {
#ifdef _WIN32
    // Initialize Winsock for Windows
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      throw std::runtime_error{"WSAStartup failed"};
    }
#endif
  }
  static void deinit() {
#ifdef _WIN32
    WSACleanup(); // Cleanup Winsock for Windows
#endif
  }
};
} // namespace socketpp
namespace libssh2pp {
class Init_handle {
public:
  Init_handle() { init(); }
  ~Init_handle() { deinit(); }
  void init() {
    auto error = libssh2_init(0);
    if (error) {
      SPDLOG_ERROR("Failed to init libssh2.");
      throw std::runtime_error{"Failed to init libssh2"};
    }
  }
  void deinit() { libssh2_exit(); }
};

class Session {
public:
  Session() { init(); }
  void init() {
    session_ = libssh2_session_init();
    if (!session_) {
      SPDLOG_ERROR("Failed to init libssh2 session.");
      throw std::runtime_error{"Failed to init libssh2 session"};
    }
  }
  void deinit() {}
  LIBSSH2_SESSION *session_{};
};

} // namespace libssh2pp

int main(int argc, char **argv) {
  socketpp::Network_init_handle network_init;
  libssh2pp::Init_handle libssh2;
  libssh2pp::Session session;

  // Set up connection options
  std::string hostname = argc > 1 ? argv[1] : "127.0.0.1";
  int port = 22;

  /* Enable all debugging when libssh2 was built with debugging enabled */
  libssh2_trace(session.session_, ~0);

  libssh2_socket_t sock;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == LIBSSH2_INVALID_SOCKET) {
    std::cerr << "Failed to create socket" << std::endl;
  }

  // Resolve hostname and connect
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = inet_addr(hostname.c_str());
  if (connect(sock, (struct sockaddr *)(&sin), sizeof(struct sockaddr_in)) !=
      0) {
    SPDLOG_ERROR("Failed to connect to host: {}", hostname);
    throw std::runtime_error{"Failed to connect"};
  }

  auto error_code = libssh2_session_handshake(session.session_, sock);

  // if(rc) {
  //     fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
  //     goto shutdown;
  // }

  /* At this point we have not yet authenticated.  The first thing to do
   * is check the hostkey's fingerprint against our known hosts Your app
   * may have it hard coded, may go to a file, may present it to the
   * user, that's your call
   */
  auto fingerprint =
      libssh2_hostkey_hash(session.session_, LIBSSH2_HOSTKEY_HASH_SHA1);
  if (!fingerprint) {
    SPDLOG_ERROR("Failed to get peer hash");
  }

  fprintf(stderr, "Fingerprint: ");
  for (int i = 0; i < 20; i++) {
    fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
  }
  fprintf(stderr, "\n");
  std::string username = argc > 2 ? argv[2] : "root";
  /* check what authentication methods are available */
  auto userauthlist = libssh2_userauth_list(session.session_, username.c_str(),

                                            username.size());
  int auth_pw{};
  if (userauthlist) {
    fprintf(stderr, "Authentication methods: %s\n", userauthlist);
    if (strstr(userauthlist, "password")) {
      auth_pw |= 1;
    }
    if (strstr(userauthlist, "keyboard-interactive")) {
      auth_pw |= 2;
    }
    if (strstr(userauthlist, "publickey")) {
      auth_pw |= 4;
    }

    // /* check for options */
    // if (argc > 4) {
    //   if ((auth_pw & 1) && !strcmp(argv[4], "-p")) {
    //     auth_pw = 1;
    //   }
    //   if ((auth_pw & 2) && !strcmp(argv[4], "-i")) {
    //     auth_pw = 2;
    //   }
    //   if ((auth_pw & 4) && !strcmp(argv[4], "-k")) {
    //     auth_pw = 4;
    //   }
    // }
    std::string password = argc > 3 ? argv[3] : "admin";
    if (auth_pw & 1) {
      /* We could authenticate via password */
      if (libssh2_userauth_password(session.session_, username.c_str(),
                                    password.c_str())) {

        SPDLOG_ERROR("Authentication by password failed: as '{}'", username);
        throw std::runtime_error{"Failed to authentication."};
      } else {
        fprintf(stderr, "Authentication by password succeeded.\n");
      }
    } else if (auth_pw & 2) {
      //   /* Or via keyboard-interactive */
      //   if (libssh2_userauth_keyboard_interactive(session, username,

      //                                             &kbd_callback)) {
      //     fprintf(stderr, "Authentication by keyboard-interactive
      //     failed.\n"); goto shutdown;
      //   } else {
      //     fprintf(stderr, "Authentication by keyboard-interactive
      //     succeeded.\n");
      //   }
    } else if (auth_pw & 4) {
      /* Or by public key */
      //   size_t fn1sz, fn2sz;
      //   char *fn1, *fn2;
      //   char const *h = getenv("HOME");
      //   if (!h || !*h)
      //     h = ".";
      //   fn1sz = strlen(h) + strlen(pubkey) + 2;
      //   fn2sz = strlen(h) + strlen(privkey) + 2;
      //   fn1 = malloc(fn1sz);
      //   fn2 = malloc(fn2sz);
      //   if (!fn1 || !fn2) {
      //     free(fn2);
      //     free(fn1);
      //     fprintf(stderr, "out of memory\n");
      //     goto shutdown;
    }
    /* Using asprintf() here would be much cleaner,
       but less portable */
    //   snprintf(fn1, fn1sz, "%s/%s", h, pubkey);
    //   snprintf(fn2, fn2sz, "%s/%s", h, privkey);

    //   if (libssh2_userauth_publickey_fromfile(session, username,

    //                                           fn1, fn2, password)) {
    //     fprintf(stderr, "Authentication by public key failed.\n");
    //     free(fn2);
    //     free(fn1);
    //     goto shutdown;
    //   } else {
    //     fprintf(stderr, "Authentication by public key succeeded.\n");
    //   }
    //   free(fn2);
    //   free(fn1);
    // } else {
    //   fprintf(stderr, "No supported authentication methods found.\n");
    //   goto shutdown;
    // }
  }

  /* Request a session channel on which to run a shell */
  auto channel = libssh2_channel_open_session(session.session_);

  if (!channel) {
    fprintf(stderr, "Unable to open a session\n");
    std::runtime_error{"Failed to open session"};
  }

  /* Some environment variables may be set,
   * It's up to the server which ones it'll allow though
   */
  libssh2_channel_setenv(channel, "FOO", "bar");

  /* Request a terminal with 'vanilla' terminal emulation
   * See /etc/termcap for more options. This is useful when opening
   * an interactive shell.
   */
  // #if 0
  //     if(libssh2_channel_request_pty(channel, "vanilla")) {

  //         fprintf(stderr, "Failed requesting pty\n");
  //     }

  //     if(argc > 5) {
  //   if (libssh2_channel_exec(channel, "uptime")) {

  //     SPDLOG_ERROR("Unable to request command on channel");
  //     //   goto shutdown;
  //   }
  /* Instead of just running a single command with
  libssh2_channel_exec,
   * a shell can be opened on the channel instead, for interactive
   use.
   * You usually want a pty allocated first in that case (see above).
   */
  // #if 0
  if (libssh2_channel_shell(channel)) {

    fprintf(stderr, "Unable to request shell on allocated pty\n");
    //   goto shutdown;
  }

  /* At this point the shell can be interacted with using
   * libssh2_channel_read()
   * libssh2_channel_read_stderr()
   * libssh2_channel_write()
   * libssh2_channel_write_stderr()
   *
   * Blocking mode may be (en|dis)abled with:
   *    libssh2_channel_set_blocking()
   * If the server send EOF, libssh2_channel_eof() will return non-0
   * To send EOF to the server use: libssh2_channel_send_eof()
   * A channel can be closed with: libssh2_channel_close()
   * A channel can be freed with: libssh2_channel_free()
   */

  /* Read and display all the data received on stdout (ignoring
  stderr)
   * until the channel closes. This will eventually block if the
   command
   * produces too much data on stderr; the loop must be rewritten to
   use
   * non-blocking mode and include interspersed calls to
   * libssh2_channel_read_stderr() to avoid this. See ssh2_echo.c for
   * an idea of how such a loop might look.
   */
  std::string shell_command = "uptime\n";
  libssh2_channel_write(channel, shell_command.c_str(), shell_command.size());
  while (!libssh2_channel_eof(channel)) {

    char buf[1024];
    ssize_t err = libssh2_channel_read(channel, buf, sizeof(buf));

    if (err < 0)
      fprintf(stderr, "Unable to read response: %d\n", (int)err);
    else {
      fwrite(buf, 1, err, stdout);
    }
  }
  //   }

  //   rc = libssh2_channel_get_exit_status(channel);

  //   if(libssh2_channel_close(channel))

  //       fprintf(stderr, "Unable to close channel\n");

  //   if(channel) {
  //       libssh2_channel_free(channel);

  //       channel = NULL;
  //   }

  /* Other channel types are supported via:
   * libssh2_scp_send()
   * libssh2_scp_recv2()
   * libssh2_channel_direct_tcpip()
   */

  // shutdown:

  //     if(session) {
  //         libssh2_session_disconnect(session, "Normal Shutdown");

  //         libssh2_session_free(session);

  //     }



  fprintf(stderr, "all done\n");
}