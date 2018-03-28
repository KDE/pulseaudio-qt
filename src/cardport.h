#ifndef CARDPORT_H
#define CARDPORT_H

#include "port.h"

#include <QObject>
#include "debug.h"
#include <pulse/introspect.h>

namespace PulseAudioQt {

class CardPort : public Port
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap properties READ properties NOTIFY propertiesChanged)
public:
    explicit CardPort(QObject *parent = nullptr);
    virtual ~CardPort() {}

//    int direction;                      /**< A #pa_direction enum, indicating the direction of this port. */
//    uint32_t n_profiles;                /**< Number of entries in profile array */
//    pa_card_profile_info** profiles;    /**< \deprecated Superseded by profiles2 */
//    int64_t latency_offset;             /**< Latency offset of the port that gets added to the sink/source latency when the port is active. \since 3.0 */
//    pa_card_profile_info2** profiles2;  /**< Array of pointers to available profiles, or NULL. Array is terminated by an entry set to NULL. \since 5.0 */

    void update(const pa_card_port_info *info);

    QVariantMap properties() const;

Q_SIGNALS:
    void propertiesChanged();

private:
    QVariantMap m_properties;
};


}

#endif
