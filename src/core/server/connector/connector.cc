#include "core/server/connector/connector.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <string>

#include <glog/logging.h>

#include "core/server/connector/human_connector.h"
#include "core/server/connector/pipe_connector.h"

using namespace std;

// static
unique_ptr<Connector> Connector::create(int playerId, const string& programName)
{
    CHECK(0 <= playerId && playerId < 10) << playerId;

    if (programName == "-")
        return unique_ptr<Connector>(new HumanConnector());

    if (programName.find("fifo:") == 0) {
        string::size_type colon = programName.find(":", 5);
        string uplink_fifo = programName.substr(5, colon - 5);
        string downlink_fifo = programName.substr(colon + 1);
        CHECK(mkfifo(uplink_fifo.c_str(), 0777) == 0);
        CHECK(chmod(uplink_fifo.c_str(), 0777) == 0);
        CHECK(mkfifo(downlink_fifo.c_str(), 0777) == 0);
        CHECK(chmod(downlink_fifo.c_str(), 0777) == 0);
        int uplink_fd = open(uplink_fifo.c_str(), O_RDONLY);
        CHECK(uplink_fd >= 0);
        int downlink_fd = open(downlink_fifo.c_str(), O_WRONLY);
        CHECK(downlink_fd >= 0);

        unique_ptr<Connector> connector(new PipeConnector(downlink_fd, uplink_fd));
        return connector;
    }

    // File descriptors.
    int fd_field_status[2];
    int fd_command[2];

    if (pipe(fd_field_status) < 0)
        PLOG(FATAL) << "Pipe error. ";
    if (pipe(fd_command) < 0)
        PLOG(FATAL) << "Pipe error. ";

    pid_t pid = fork();
    if (pid < 0)
        PLOG(FATAL) << "Failed to fork. ";

    if (pid > 0) {
        // Server.
        LOG(INFO) << "Created a child process (pid = " << pid << ")";

        unique_ptr<Connector> connector(new PipeConnector(fd_field_status[1], fd_command[0]));
        close(fd_field_status[0]);
        close(fd_command[1]);

        return connector;
    }

    // Client.
    if (dup2(fd_field_status[0], STDIN_FILENO) < 0)
        PLOG(FATAL) << "Failed to dup2. ";
    if (dup2(fd_command[1], STDOUT_FILENO) < 0)
        PLOG(FATAL) << "Failed to dup2. ";

    close(fd_field_status[0]);
    close(fd_field_status[1]);
    close(fd_command[0]);
    close(fd_command[1]);

    char filename[] = "Player_";
    filename[6] = '1' + playerId;

    if (execl(programName.c_str(), programName.c_str(), filename, nullptr) < 0)
        PLOG(FATAL) << "Failed to start a child process. ";

    LOG(FATAL) << "should not be reached.";
    return unique_ptr<Connector>();
}
