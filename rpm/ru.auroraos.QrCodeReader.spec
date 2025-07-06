Name: ru.auroraos.QrCodeReader
Summary: QR Code Reader
Version: 0.1.0
Release: 1
License: BSD-3-Clause
Source0: %{name}-%{version}.tar.bz2

# Aurora SDK version
%define aurora_sdk_version %(cat /etc/os-release | grep VERSION_ID | cut -d '=' -f2)
# Parsing aurora_sdk_version by number
# 1. major version sdk
%define aurora_sdk_major_version %(echo %{aurora_sdk_version} | cut -f1 -d".")
# 2. minor version sdk
%define aurora_sdk_minor_version %(echo %{aurora_sdk_version} | cut -f2 -d".")
# 3. build version sdk
%define aurora_sdk_build_version %(echo %{aurora_sdk_version} | cut -f3 -d".")
# 4. revision version sdk
%define aurora_sdk_revision_version %(echo %{aurora_sdk_version} | cut -f4 -d".")

%if "%{aurora_sdk_major_version}" == "4" && "%{aurora_sdk_revision_version}" < "174"
%define libapp sailfishapp
%else
%define libapp auroraapp
%endif

BuildRequires: pkgconfig(%{libapp})
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5Network)
BuildRequires: pkgconfig(Qt5Location)
BuildRequires: pkgconfig(Qt5Positioning)
BuildRequires: qr-filter-qml-plugin >= 1.1.0
Requires: sailfishsilica-qt5 >= 0.10.9
Requires: qr-filter-qml-plugin >= 1.1.0
Requires: qt5-qtlocation
Requires: qt5-qtpositioning
Requires: qt5-qtdeclarative-import-location
Requires: qt5-qtdeclarative-import-positioning
Requires: qt5-plugin-geoservices-webtiles
Requires: qt5-qtgraphicaleffects

%description
The project provides an example of using video filters to process QR codes

%prep
%autosetup

%build
%qmake5 \
AURORA_SDK_VERSION=%{aurora_sdk_version} \
AURORA_SDK_MAJOR_VERSION=%{aurora_sdk_major_version} \
AURORA_SDK_MINOR_VERSION=%{aurora_sdk_minor_version} \
AURORA_SDK_BUILD_VERSION=%{aurora_sdk_build_version} \
AURORA_SDK_REVISION_VERSION=%{aurora_sdk_revision_version}
%make_build

%install
%make_install

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
