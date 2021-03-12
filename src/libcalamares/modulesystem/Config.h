/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2021 Adriaan de Groot <groot@kde.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#ifndef CALAMARES_MODULESYSTEM_CONFIG_H
#define CALAMARES_MODULESYSTEM_CONFIG_H

#include "DllMacro.h"

#include <QObject>
#include <QStringList>
#include <QVariantMap>

#include <memory>

namespace Calamares
{
namespace ModuleSystem
{
/** @brief Base class for Config-objects
 *
 * This centralizes the things every Config-object should
 * do and provides one source of preset-data. A Config-object
 * for a module can **optionally** inherit from this class
 * to get presets-support.
 *
 * TODO:3.3 This is not optional
 * TODO:3.3 Put consistent i18n for Configurations in here too
 */
class DLLEXPORT Config : public QObject
{
    Q_OBJECT
public:
    Config( QObject* parent = nullptr );
    ~Config() override;

    /** @brief Set the configuration from the config file
     *
     * Subclasses must implement this to load configuration data;
     * that subclass **should** also call loadPresets() with the
     * same map, to pick up the "presets" key consistently.
     */
    virtual void setConfigurationMap( const QVariantMap& ) = 0;

public Q_SLOTS:
    /** @brief Checks if a @p fieldName is editable according to presets
     *
     * If the field is named as a preset, **and** the field is set
     * to not-editable, returns @c false. Otherwise, return @c true.
     * Calling this with an unknown field (one for which no presets
     * are accepted) will print a warning and return @c true.
     */
    bool isEditable( const QString& fieldName ) const;

protected:
    friend class ApplyPresets;
    /** @brief "Builder" class for presets
     *
     * Derived classes should instantiate this (with themselves,
     * and the whole configuration map that is passed to
     * setConfigurationMap()) and then call .apply() to apply
     * the presets specified in the configuration to the **named**
     * QObject properties.
     */
    class ApplyPresets
    {
    public:
        ApplyPresets( Config& c, const QVariantMap& configurationMap );
        ~ApplyPresets() { m_c.m_unlocked = false; }

        ApplyPresets& apply( const char* fieldName );

    private:
        Config& m_c;
        bool m_bogus = true;
        const QVariantMap m_map;
    };

private:
    class Private;
    std::unique_ptr< Private > d;
    bool m_unlocked = false;
};
}  // namespace ModuleSystem
}  // namespace Calamares

#endif
