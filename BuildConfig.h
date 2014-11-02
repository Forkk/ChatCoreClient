#pragma once
#include <QString>

/**
 * \brief The Config class holds all the build-time information passed from the build system.
 */
class Config
{
public:
	Config();
	/// The major version number.
	int VERSION_MAJOR;
	/// The minor version number.
	int VERSION_MINOR;
	/// The hotfix number.
	int VERSION_HOTFIX;
	/// The build number.
	int VERSION_BUILD;

    /// Git commit hash
    QString GIT_COMMIT;

	/// Used for matching notifications
	QString FULL_VERSION_STR;

	/// This is printed on start to standard output
	QString VERSION_STR;

	/**
	 * \brief Converts the Version to a string.
	 * \return The version number in string format (major.minor.revision.build).
	 */
	QString printableVersionString() const;
};

extern Config BuildConfig;
