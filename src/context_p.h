#ifndef CONTEXT_P_H
#define CONTEXT_P_H

#include "maps.h"
#include "operation.h"
#include <pulse/context.h>
#include <pulse/introspect.h>
#include <qglobal.h>
#include <pulse/ext-stream-restore.h>
#include <pulse/glib-mainloop.h>

namespace PulseAudioQt
{

class ContextPrivate {
public:
    
    explicit ContextPrivate(Context* q);
    virtual ~ContextPrivate();
    
    // Don't forget to add things to reset().
    SinkMap m_sinks;
    SinkInputMap m_sinkInputs;
    SourceMap m_sources;
    SourceOutputMap m_sourceOutputs;
    ClientMap m_clients;
    CardMap m_cards;
    ModuleMap m_modules;
    StreamRestoreMap m_streamRestores;
    Server *m_server;

    pa_context *m_context;
    pa_glib_mainloop *m_mainloop;

    QString m_newDefaultSink;
    QString m_newDefaultSource;

    int m_references;

    void subscribeCallback(pa_context *context, pa_subscription_event_type_t type, uint32_t index);
    void contextStateCallback(pa_context *context);
    void sinkCallback(const pa_sink_info *info);
    void sinkInputCallback(const pa_sink_input_info *info);
    void sourceCallback(const pa_source_info *info);
    void sourceOutputCallback(const pa_source_output_info *info);
    void clientCallback(const pa_client_info *info);
    void cardCallback(const pa_card_info *info);
    void moduleCallback(const pa_module_info *info);
    void streamRestoreCallback(const pa_ext_stream_restore_info *info);
    void serverCallback(const pa_server_info *info);
    void streamRestoreWrite(const pa_ext_stream_restore_info *info);
    void setGenericVolume(quint32 index, int channel, qint64 newVolume, pa_cvolume cVolume, const std::function<pa_operation*(pa_context *, uint32_t, const pa_cvolume *, pa_context_success_cb_t, void *)>& );
    void setGenericMute(quint32 index, bool mute, const std::function<pa_operation*(pa_context *, uint32_t, int, pa_context_success_cb_t, void *)>& );
    void setGenericPort(quint32 index, const QString &portName, const std::function<pa_operation*(pa_context *, uint32_t, const char*, pa_context_success_cb_t, void *)>& );
    void setGenericDeviceForStream(quint32 streamIndex, quint32 deviceIndex, const std::function<pa_operation*(pa_context *, uint32_t, uint32_t, pa_context_success_cb_t, void *)>& );
    void reset();
    void connectToDaemon();

    Context* q;
        
};

}
#endif
