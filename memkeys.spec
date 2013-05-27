Name:           memkeys
Version:        0.2
Release:        1%{?dist}
Summary:        Shows your memcache key usage in realtime

Group:          Development/Tools
License:        Apache 2.0
URL:            https://github.com/tumblr/memkeys
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

# Uses 'preview' GCC 4.4 on RHEL 5 for needed features.
%if 0%{?rhel} < 6
BuildRequires:  gcc44
BuildRequires:  gcc44-c++
%else
BuildRequires:  gcc
BuildRequires:  gcc-c++
%endif

BuildRequires:  libtool >= 1.5.22
BuildRequires:  autoconf >= 2.61
BuildRequires:  automake >= 1.9.6
BuildRequires:  libpcap-devel >= 0.9.4-15
BuildRequires:  pcre-devel >= 6.6-6
BuildRequires:  ncurses-devel >= 5.5-24.20060715

Requires:       libpcap >= 0.9.4-15
Requires:       pcre >= 6.6-6
Requires:       ncurses >= 5.5-24.20060715


%description
memkeys provides an interface to track the usage of memcache keys in realtime,
originally inspired by mctop from mctop from etsy.  It provides an ncurses
interface alongside a CSV reporting interface, and is designed to drop less
than 3 percent of memcache packets at production load, tested on a saturated 1
Gigabit link.


%prep
%setup -q


%build
%if 0%{?rhel} < 6
export CC=gcc44
export CXX=g++44
%endif

./build-eng/autogen.sh
%configure

make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc LICENSE NOTICE README.md 
%{_bindir}/memkeys


%changelog
* Sun Apr 28 2013 Steve Salevan <steve@tumblr.com> 0.2-1
- Initial specfile writeup


