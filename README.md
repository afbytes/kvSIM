kvSIM is a patch which simulates "Multi SIM in One" (Magic Sim) for
Siemens SL45i cellphone.


What is this?
=============

"Multi SIM in One" (Magic Sim) need a special SIM card to store 'KI+IMSI'
that be read through a 'SIM card Reader'.

kvSIM make it possible to implement the function of "Multi SIM in One"
with a normal SIM card - but it still need a SIM card Reader to read
the 'KI+IMSI' recently.


How to compile the source?
==========================

1. You should use "TASKING Tools for C166" to compile the source.
2. Just run "hex2vkp.bat" to generate a file named "kvSIM.vkp".
3. You should compare "kvSIM.vkp" with "kvSIM_090.vkp" to confirm
   what else you should add or modify to the patch beyond the source.


LICENSE
=======

    kvSIM patch for Siemens 6688i(SL45i) v55lg8
    Copyright (C) 2004-2007 Konca Fung.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


Contact
=======

Konca Fung <koncafung@gmail.com> (http://www.konca.com)

