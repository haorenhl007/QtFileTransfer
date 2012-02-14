#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include <QFile>
#include "../fileinfo.h"
#include "../sharedstructures.h"
#include <QTimer>

#define HISTORY_SIZE	10

class ServerObject : public QObject
{
	Q_OBJECT
public:
	ServerObject(int socketDescriptor, QList<FileInfo*> *file_list, QObject *parent = 0);

	
signals:
	void error(QTcpSocket::SocketError socketError);
	void finished();

	void progressUpdate(qint64 bytes_sent, double speed, ServerObject *obj);
	void fileTransferBeginning(FileInfo *file, ServerObject *obj, QString ip);
	void fileListRequested();
	void fileTransferCompleted(ServerObject *obj);
	void fileTransferAborted(ServerObject *obj);
	void fileListTransferCompleted();

public slots:
	void handleConnection();
	void quitRequest();

private slots:
	void readReady();
	void sendNextListItem(qint64 bytes);
	void sendNextFilePiece(qint64 bytes);

	void disconnected();
	void fileTransferSocketError(QAbstractSocket::SocketError err);

	void triggerUIupdate();

private:

	void listRequest();
	void fileRequest(connControlMsg msg);


	enum opMode { NONE, SENDING_FILE, SENDING_LIST };


	opMode m_currentMode;
	int m_socketDescriptor;
	QTcpSocket *m_socket;

	QList<FileInfo*> *m_fileList;
	int m_items_sent;


	QFile *m_file;
	FileInfo *send_file;
	qint64 bytes_sent_total;

	QTime *timer;
	int running_byte_total;
	int running_ms_total;
	int prev_bytes_sent[HISTORY_SIZE];
	int prev_times_sent[HISTORY_SIZE];
	int head_index;

	void update_speed(int bytes_sent, int ms);
	double getSpeed();

	QTimer *regular_ui_updates;
	
};

#endif // SERVERTHREAD_H
