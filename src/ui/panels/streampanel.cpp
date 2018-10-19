#include "streampanel.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <ui/widgets/selectbox.hpp>
#include <ui/widgets/inputbox.hpp>
#include <ui/widgets/outputbox.hpp>

StreamPanel::StreamPanel(VlcManager *vlcManager,
                         QWidget *parent)
    : m_vlcManager(vlcManager)
{
    // Enable check box
    m_transcodeEnableCheckBox = new QCheckBox("Enable");
    m_transcodeEnableCheckBox->setChecked(true);

    // Video codec select box
    m_vcodecSelectBox = new SelectBox("Codec");
    m_vcodecSelectBox->addItem("None"  , Vlc::NoVideo);
    m_vcodecSelectBox->addItem("MPEG2V", Vlc::MPEG2Video);
    m_vcodecSelectBox->addItem("MPEG4V", Vlc::MPEG4Video);
    m_vcodecSelectBox->addItem("H.264" , Vlc::H264);
    m_vcodecSelectBox->addItem("Theora", Vlc::Theora);

    // Quality select box
    m_qualitySelectBox = new SelectBox("Quality");
    m_qualitySelectBox->addItem("240" , 240);
    m_qualitySelectBox->addItem("360" , 360);
    m_qualitySelectBox->addItem("480" , 480);
    m_qualitySelectBox->addItem("720" , 720);
    m_qualitySelectBox->addItem("1080", 1080);

    // Aspect ratio select box
    m_aspectRatioSelectBox = new SelectBox("Aspect ratio");
    m_aspectRatioSelectBox->addItem("Original", Vlc::Original);
    m_aspectRatioSelectBox->addItem("4:3"     , Vlc::R_4_3);
    m_aspectRatioSelectBox->addItem("16:9"    , Vlc::R_16_9);
    m_aspectRatioSelectBox->addItem("16:10"   , Vlc::R_16_10);

    // Video bitrate input box
    m_vbitrateInputBox = new InputBox("Bitrate");

    // Fps input box
    m_frameRateInputBox = new InputBox("Frame rate");

    // Video layout
    m_videoLayout = new QVBoxLayout;
    m_videoLayout->setMargin(0);
    m_videoLayout->addWidget(m_vcodecSelectBox);
    m_videoLayout->addWidget(m_qualitySelectBox);
    m_videoLayout->addWidget(m_aspectRatioSelectBox);
    m_videoLayout->addWidget(m_vbitrateInputBox);
    m_videoLayout->addWidget(m_frameRateInputBox);

    // Video group box
    m_videoGroupBox = new QGroupBox("Video");
    m_videoGroupBox->setContentsMargins(10, 15, 10, 10);
    m_videoGroupBox->setLayout(m_videoLayout);

    // Audio codec
    m_acodecSelectBox = new SelectBox("Codec");
    m_acodecSelectBox->addItem("None"  , Vlc::NoAudio);
    m_acodecSelectBox->addItem("MPEG2A", Vlc::MPEG2Audio);
    m_acodecSelectBox->addItem("MP3"   , Vlc::MP3);
    m_acodecSelectBox->addItem("MPEG4A", Vlc::MPEG4Audio);
    m_acodecSelectBox->addItem("Vorbis", Vlc::Vorbis);
    m_acodecSelectBox->addItem("Flac"  , Vlc::Flac);

    // Audio bitrate
    m_abitrateInputBox = new InputBox("Bitrate");

    // Audio sample rate
    m_sampleRateInputBox = new InputBox("Sample rate");

    // Audio layout
    m_audioLayout = new QVBoxLayout;
    m_audioLayout->setMargin(0);
    m_audioLayout->addWidget(m_acodecSelectBox);
    m_audioLayout->addWidget(m_abitrateInputBox);
    m_audioLayout->addWidget(m_sampleRateInputBox);

    // Audio group box
    m_audioGroupBox = new QGroupBox("Audio");
    m_audioGroupBox->setContentsMargins(10, 15, 10, 10);
    m_audioGroupBox->setLayout(m_audioLayout);

    // Transcode layout
    m_transcodeLayout = new QVBoxLayout;
    m_transcodeLayout->setMargin(0);
    m_transcodeLayout->addWidget(m_transcodeEnableCheckBox);
    m_transcodeLayout->addWidget(m_videoGroupBox);
    m_transcodeLayout->addWidget(m_audioGroupBox);

    // Transcode group box
    m_transcodeGroupBox = new QGroupBox("Transcode");
    m_transcodeGroupBox->setContentsMargins(10, 15, 10, 10);
    m_transcodeGroupBox->setLayout(m_transcodeLayout);

    /* Ip input box */
    m_ipInputBox = new InputBox("Ip");

    /* Port input box */
    m_portInputBox = new InputBox;

    /* SAP check box */
    m_sapCheckBox = new QCheckBox("SAP");
    m_sapCheckBox->setChecked(false);

    /* Name input */
    m_nameInputBox = new InputBox("Name");

    /* Address layout */
    m_addressLayout = new QHBoxLayout;
    m_addressLayout->setMargin(0);
    m_addressLayout->addWidget(m_ipInputBox);
    m_addressLayout->addWidget(m_portInputBox);

    /* Start stream button */
    m_runStreamButton = new QPushButton("Start stream");

    /* Main layout */
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainLayout->addWidget(m_transcodeGroupBox);
    m_mainLayout->addWidget(m_nameInputBox);
    m_mainLayout->addLayout(m_addressLayout);
    m_mainLayout->addWidget(m_sapCheckBox);
    m_mainLayout->addWidget(m_runStreamButton);

    /* Widget */
    setLayout(m_mainLayout);
    setEnabled(false);

    /* Connections */
    connect(m_runStreamButton, &QPushButton::clicked, this, &StreamPanel::slotPlayStream);
    connect(m_transcodeEnableCheckBox, &QCheckBox::toggled, this, &StreamPanel::slotTranscodeSetEnabled);
    connect(m_vlcManager, &VlcManager::mediaSetted, this, &StreamPanel::setEnabled);
}

StreamPanel::~StreamPanel()
{
}

void StreamPanel::setParameters()
{
    SoutBuilder &builder = m_vlcManager->getSoutBuilder();

    // Set transcode parameters
    builder.setTranscodeEnabled(m_transcodeEnableCheckBox->isChecked());
    if (m_transcodeEnableCheckBox->isChecked()) {
        builder.setTrcVideoCodec(static_cast<Vlc::VideoCodec>(m_vcodecSelectBox->currentData().toInt()));
        builder.setTrcVideoQuality(m_qualitySelectBox->currentData().toInt());
        builder.setTrcVideoAspectRatio(static_cast<Vlc::Ratio>(m_aspectRatioSelectBox->currentData().toInt()));
        builder.setTrcVideoBitrate(m_vbitrateInputBox->getText().toInt());
        builder.setTrcVideoFrameRate(m_frameRateInputBox->getText().toInt());
        builder.setTrcAudioCodec(static_cast<Vlc::AudioCodec>(m_acodecSelectBox->currentData().toInt()));
        builder.setTrcAudioBitrate(m_abitrateInputBox->getText().toInt());
        builder.setTrcAudioSampleRate(m_sampleRateInputBox->getText().toInt());
    }

    // Set rtp parameters
    builder.setRtpName(m_nameInputBox->getText());
    builder.setRtpIp(m_ipInputBox->getText());
    builder.setRtpPort(m_portInputBox->getText().toInt());
    builder.setSapEnabled(m_sapCheckBox->isChecked());
}

void StreamPanel::slotPlayStream()
{
    setParameters();
    m_vlcManager->playStream();
}

void StreamPanel::slotTranscodeSetEnabled(bool enabled)
{
    m_videoGroupBox->setEnabled(enabled);
    m_audioGroupBox->setEnabled(enabled);
}