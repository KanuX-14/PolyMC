#pragma once

#include <QDateTime>
#include <QFileInfo>
#include <QObject>

#include "QObjectPtr.h"

enum class ResourceType {
    UNKNOWN,     //!< Indicates an unspecified resource type.
    ZIPFILE,     //!< The resource is a zip file containing the resource's class files.
    SINGLEFILE,  //!< The resource is a single file (not a zip file).
    FOLDER,      //!< The resource is in a folder on the filesystem.
    LITEMOD,     //!< The resource is a litemod
};

/** General class for managed resources. It mirrors a file in disk, with some more info
 *  for display and house-keeping purposes.
 *
 *  Subclass it to add additional data / behavior, such as Mods or Resource packs.
 */
class Resource : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Resource)
   public:
    using Ptr = shared_qobject_ptr<Resource>;

    Resource(QObject* parent = nullptr);
    Resource(QFileInfo file_info);
    ~Resource() override = default;

    void setFile(QFileInfo file_info);
    void parseFile();

    [[nodiscard]] auto fileinfo() const -> QFileInfo { return m_file_info; }
    [[nodiscard]] auto dateTimeChanged() const -> QDateTime { return m_changed_date_time; }
    [[nodiscard]] auto internal_id() const -> QString { return m_internal_id; }
    [[nodiscard]] auto type() const -> ResourceType { return m_type; }

    [[nodiscard]] virtual auto name() const -> QString { return m_name; }
    [[nodiscard]] virtual bool valid() const { return m_type != ResourceType::UNKNOWN; }

    [[nodiscard]] auto shouldResolve() const -> bool { return !m_is_resolving && !m_is_resolved; }
    [[nodiscard]] auto isResolving() const -> bool { return m_is_resolving; }
    [[nodiscard]] auto resolutionTicket() const -> int { return m_resolution_ticket; }

    void setResolving(bool resolving, int resolutionTicket)
    {
        m_is_resolving = resolving;
        m_resolution_ticket = resolutionTicket;
    }

    // Delete all files of this resource.
    bool destroy();

   protected:
    /* The file corresponding to this resource. */
    QFileInfo m_file_info;
    /* The cached date when this file was last changed. */
    QDateTime m_changed_date_time;

    /* Internal ID for internal purposes. Properties such as human-readability should not be assumed. */
    QString m_internal_id;
    /* Name as reported via the file name. In the absence of a better name, this is shown to the user. */
    QString m_name;

    /* The type of file we're dealing with. */
    ResourceType m_type = ResourceType::UNKNOWN;

    /* Used to keep trach of pending / concluded actions on the resource. */
    bool m_is_resolving = false;
    bool m_is_resolved = false;
    int m_resolution_ticket = 0;
};
