/* 
 * Block.java:  Class describing "blocks", base class for 
 *              just about every database entry.
 *
 * Copyright (C) 1997, 1998, Kenneth Albanowski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

package Pdapilot;

import java.io.*;

/** A representation of a generic database block.
 */

public class Block {
		public byte[] raw;
		
		public Block() {
			this.fill();
		}
		public Block(byte[] contents) {
			this.unpack(contents);
		}
		
		public void fill() {
		}
		
		public byte[] pack() {
			return raw;
		}
		
		public void unpack(byte[] data) {
			raw = data;
		}
		
		public String describe() {
			return "raw='" + Util.prettyPrint(raw);
		}

	public String toString() {
		return "<" + this.getClass().getName() + " " + describe() + ">";
	}
}
