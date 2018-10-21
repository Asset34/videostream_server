#ifndef VLCMANAGER_HPP
#define VLCMANAGER_HPP

#include <QObject>
#include <QTime>

#include <soutbuilder.hpp>

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class VlcManager : public QObject
{
    Q_OBJECT

public:
    explicit VlcManager(QObject *parent = nullptr);
    virtual ~VlcManager();

    void setMedia(const QString &path);

    QString getMediaPath() const;
    QString getMediaTitle() const;
    QString getMediaDescription() const;
    QTime getMediaDuration() const;

    SoutBuilder &getSoutBuilder();

    void playStream();
    void pauseStream();
    void resumeStream();
    void stopStream();

private:
    bool checkMediaFile(const QString &path) const;

    void createMedia(const QString &path);
    void clearMedia();
    void resetMedia(const QString &path);
    void parseMedia();

    VlcInstance *m_instance;
    VlcMedia *m_media;
    VlcMediaPlayer *m_mediaPlayer;

    SoutBuilder m_soutBuilder;

signals:
    void mediaSetted(bool setted);
    void mediaStateChanged(QString state);
    void mediaTimeChanged(QTime time);
    void errorOccured(QString error);

};

#endif // VLCMANAGER_HPP
