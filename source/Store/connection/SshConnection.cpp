// Panduza
#include "SshConnection.hpp"
#include <Store/Store.hpp>

// Qt
#include <QDebug>
#include <QtConcurrent>


// ============================================================================
//
int SshConnection::remoteExec(const QString& cmd, QByteArray& stdout)
{
    qDebug() << "remoteExec";    
    

    ssh_channel channel;
    int rc = 0;
    char buffer[256];
    int nbytes;

    stdout.clear();
 
    channel = ssh_channel_new(mSshSession);
    if (channel == NULL)
    {
        qDebug() << "error";
        return rc;
    }

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK)
    {
        ssh_channel_free(channel);
        qDebug() << "error";    
        return rc;
    }
 
  rc = ssh_channel_request_exec(channel, cmd.toStdString().c_str() );
  if (rc != SSH_OK)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    qDebug() << "error";
    return rc;
  }
 
  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  while (nbytes > 0)
  {
        stdout.append(buffer, nbytes);
    // if (write(1, buffer, nbytes) != (unsigned int) nbytes)
    // {
    //   ssh_channel_close(channel);
    //   ssh_channel_free(channel);
    //   return SSH_ERROR;
    // }
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  }
 
  if (nbytes < 0)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return SSH_ERROR;
  }
 
  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
 
  return SSH_OK;
}

// ============================================================================
//
bool SshConnection::isRemoteFileExist(const QString& remote_path)
{
    auto bash_line = QString("if test -f \"%1\"; then echo \"yes\"; else echo \"no\"; fi").arg(remote_path);

    QByteArray stdout;
    remoteExec(bash_line, stdout);

    qDebug() << QString(stdout);
    if(QString(stdout) == "yes\n")
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ============================================================================
//
int SshConnection::getFile(const QString& remote_path, QByteArray& content)
{
    ssh_scp scp;
    int rc;

    scp = ssh_scp_new(mSshSession, SSH_SCP_READ, remote_path.toStdString().c_str() );
    if (scp == NULL)
    {
        fprintf(stderr, "Error allocating scp session: %s\n",
                ssh_get_error(mSshSession));
        return SSH_ERROR;
    }

    rc = ssh_scp_init(scp);
    if (rc != SSH_OK)
    {
    fprintf(stderr, "Error initializing scp session: %s\n",
            ssh_get_error(mSshSession));
    ssh_scp_free(scp);
    return rc;
    }

    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE)
    {
    fprintf(stderr, "Error receiving information about file: %s\n",
            ssh_get_error(mSshSession));
    return SSH_ERROR;
    }


    char *filename;
    auto size = ssh_scp_request_get_size(scp);
    filename = strdup(ssh_scp_request_get_filename(scp));
    auto mode = ssh_scp_request_get_permissions(scp);
    printf("Receiving file %s, size %d, permissions 0%o\n",
                filename, size, mode);
    free(filename);



    

    content.resize(size);

    ssh_scp_accept_request(scp);
    rc = ssh_scp_read(scp, content.data(), size);
    if (rc == SSH_ERROR)
    {
    fprintf(stderr, "Error receiving file data: %s\n",
            ssh_get_error(mSshSession));
    // free(buffer);
    return rc;
    }
    printf("Done\n");


    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_EOF)
    {
    fprintf(stderr, "Unexpected request: %s\n",
            ssh_get_error(mSshSession));
    return SSH_ERROR;
    }


    ssh_scp_close(scp);
    ssh_scp_free(scp);
    return SSH_OK;
}




// ============================================================================
//
void SshConnection::requestRestart()
{
    mRestartFlag = true;
    nextWork();
}

// ============================================================================
//
void SshConnection::nextWork()
{
    /**
     *  @todo append mutex here
     */

    // 
    if(mWorking)
    {
        qDebug() << "SSH: already working";
        return;
    }

    // 
    if(mRestartFlag) {
        asyncRestart();
    }
}

// ============================================================================
//
void SshConnection::setErrString(const QString& err)
{
    mErrString = err;
    qDebug() << err;
    emit statusChanged();
}

// ============================================================================
//
void SshConnection::asyncRestart()
{
    // Logs
    qDebug() << "SSH: Restart Connection !";

    // Lock
    mWorking = true;

    // Start the connection
    auto future = QtConcurrent::run([this] {
        restart();
    }).then([this]() {
        mWorking = false;
    });
}

// ============================================================================
//
void SshConnection::restart()
{
    stop();
    start();
}

// ============================================================================
//
void SshConnection::stop()
{
    mReady = false;

    // No pointer => nothing to stop
    if(mSshSession == nullptr) return;

    // Clear the connection if already exist
    if(ssh_is_connected(mSshSession))
    {
        ssh_disconnect(mSshSession);
    }

    // Free pointer
    ssh_free(mSshSession);
    mSshSession = nullptr;
}

// ============================================================================
//
void SshConnection::start()
{
    // Create object
    mSshSession = ssh_new();

    // Logs
    qDebug() << "SSH: Start Connection !";

    // Return code
    int rc;

    // Verbosity
    // int verbosity = SSH_LOG_NOLOG; 	    // No logging at all
    // int verbosity = SSH_LOG_WARNING; 	// Only warnings
    int verbosity = SSH_LOG_PROTOCOL; 	    // High level protocol information
    // int verbosity = SSH_LOG_PACKET; 	    // Lower level protocol infomations, packet level
    // int verbosity = SSH_LOG_FUNCTIONS;   // Every function path 

    qDebug() << "SSH: !!!";

    // //
    // QString host = Store::Get().connection.active.hostAddress();
    // int port = Store::Get().connection.active.sshPort();
    // QString username = Store::Get().connection.active.sshUsername();

    // // Logs
    // qDebug() << "SSH: Host (" << host << ")";
    // qDebug() << "SSH: Port (" << port << ")";
    // qDebug() << "SSH: User (" << username << ")";

    // // Set session credential
    // if( ssh_options_set(mSshSession, SSH_OPTIONS_HOST, host.toStdString().c_str()) < 0 )
    // {
    //     setErrString( QString("Cannot set host: %1").arg(host) );
    //     stop();
    //     return;        
    // }
    // if( ssh_options_set(mSshSession, SSH_OPTIONS_PORT, &port) < 0 )
    // {
    //     setErrString( QString("Cannot set port: %1").arg(port) );
    //     stop();
    //     return;
    // }
    // if( ssh_options_set(mSshSession, SSH_OPTIONS_USER, username.toStdString().c_str()) )
    // {
    //     setErrString( QString("Cannot set username: %1").arg(username) );
    //     stop();
    //     return;
    // }
    // if( ssh_options_set(mSshSession, SSH_OPTIONS_LOG_VERBOSITY, &verbosity) )
    // {
    //     setErrString( QString("Cannot use verbosity: %1").arg(verbosity) );
    //     stop();
    //     return;
    // }

    // // Connection
    // rc = ssh_connect(mSshSession);
    // if (rc != SSH_OK)
    // {
    //     setErrString( QString("Error connecting to host: %1").arg(ssh_get_error(mSshSession)) );
    //     stop();
    //     return;
    // }

    // // Verify the server's identity
    // // For the source code of verifyKnownhost(), check previous example
    // if (verifyKnownhost(mSshSession) < 0)
    // {
    //     setErrString( QString("Error verify known host: %1").arg(ssh_get_error(mSshSession)) );
    //     stop();
    //     return;
    // }

    // // Authenticate ourselves
    // rc = ssh_userauth_password(mSshSession, NULL, Store::Get().connection.active.sshPassword().toStdString().c_str());
    // if (rc != SSH_AUTH_SUCCESS)
    // {
    //     setErrString( QString("Error authenticating with password: %1").arg(ssh_get_error(mSshSession)) );
    //     stop();
    //     return;
    // }

    // // 
    // mReady = true;
    // qDebug() << "SSH: Connection ok !";
    // setErrString();
}

// ============================================================================
//
int SshConnection::verifyKnownhost(ssh_session session)
{
    enum ssh_known_hosts_e state;
    unsigned char *hash = NULL;
    ssh_key srv_pubkey = NULL;
    size_t hlen;
    char buf[10];
    char *hexa;
    char *p;
    int cmp;
    int rc;
 
    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if (rc < 0) {
        return -1;
    }
 
    rc = ssh_get_publickey_hash(srv_pubkey,
                                SSH_PUBLICKEY_HASH_SHA1,
                                &hash,
                                &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0) {
        return -1;
    }
 
    state = ssh_session_is_known_server(session);
    switch (state) {
        case SSH_KNOWN_HOSTS_OK:
            /* OK */
 
            break;
        case SSH_KNOWN_HOSTS_CHANGED:
            fprintf(stderr, "Host key for server changed: it is now:\n");
            // ssh_print_hexa("Public key hash", hash, hlen);
            fprintf(stderr, "For security reasons, connection will be stopped\n");
            ssh_clean_pubkey_hash(&hash);
 
            return -1;
        case SSH_KNOWN_HOSTS_OTHER:
            fprintf(stderr, "The host key for this server was not found but an other"
                    "type of key exists.\n");
            fprintf(stderr, "An attacker might change the default server key to"
                    "confuse your client into thinking the key does not exist\n");
            ssh_clean_pubkey_hash(&hash);
 
            return -1;
        case SSH_KNOWN_HOSTS_NOT_FOUND:
            fprintf(stderr, "Could not find known host file.\n");
            fprintf(stderr, "If you accept the host key here, the file will be"
                    "automatically created.\n");
 
            /* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */
 
        case SSH_KNOWN_HOSTS_UNKNOWN:
            hexa = ssh_get_hexa(hash, hlen);
            fprintf(stderr,"The server is unknown. Do you trust the host key?\n");
            fprintf(stderr, "Public key hash: %s\n", hexa);
            ssh_string_free_char(hexa);
            ssh_clean_pubkey_hash(&hash);
            // p = fgets(buf, sizeof(buf), stdin);
            // if (p == NULL) {
            //     return -1;
            // }
 
            // cmp = strncasecmp(buf, "yes", 3);
            // if (cmp != 0) {
            //     return -1;
            // }
 
            rc = ssh_session_update_known_hosts(session);
            if (rc < 0) {
                fprintf(stderr, "Error %s\n", strerror(errno));
                return -1;
            }
 
            break;
        case SSH_KNOWN_HOSTS_ERROR:
            fprintf(stderr, "Error %s", ssh_get_error(session));
            ssh_clean_pubkey_hash(&hash);
            return -1;
    }
 
    ssh_clean_pubkey_hash(&hash);
    return 0;
}


