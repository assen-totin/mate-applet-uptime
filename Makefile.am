# This file is a part of mate-applet-uptime.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

SUBDIRS = src po images

dist_doc_DATA = AUTHORS BUGS ChangeLog COPYING INSTALL NEWS README TODO

rpm:
	rpmbuild -bb --define "_builddir @TOPLEVELDIR@" mate-applet-uptime.spec
 
