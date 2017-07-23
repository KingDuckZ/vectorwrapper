 #
 # Copyright 2015-2017 Michele "King_DuckZ" Santullo
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.

import gdb.printing

#NOTE:
#This pretty-printer has been written for vectorwrapper + std::array.
#You will have to customize it if your wrapped type is different.

class VectorWrapperStdArrPrinter(object):
    "Print values in a VectorWrapper<std::array>"

    class _iterator(object):
        def __init__(self, start, size):
            self.item = start
            self.pos = 0
            self.size = size

        def __iter__(self):
            return self

        def __next__(self):
            if self.pos == self.size:
                raise StopIteration
            elt = self.item.dereference()
            self.item = self.item + 1
            self.pos = self.pos + 1
            return str(elt)

    def __init__(self, val):
        self.val = val
        self.dimensions = int(val.type.template_argument(1))

    def to_string(self):
        #get the scalar[n] value
        stdarray_elems = self.val['m_wrapped']['_M_elems']
        #get the actual scalar type
        elem_type = self.val['m_wrapped'].type.template_argument(0)
        #cast scalar[n] to scalar*
        ptr = stdarray_elems.address.cast(elem_type.pointer())
        #iterate over the values in scalar* and str() them
        retval = "vec" + str(self.dimensions) + "<" + \
            ", ".join(
                str(itm) for itm in self._iterator(ptr, self.dimensions)
            ) + ">"
        return retval

    def display_hint(self):
        return 'string'

def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter(__name__)
    #add your namespace before 'vwr' if you're using a custom one
    pp.add_printer('vwr', '^vwr::Vec<std::array.+$', VectorWrapperStdArrPrinter)
    return pp

gdb.printing.register_pretty_printer(
    gdb.current_objfile(),
    build_pretty_printer()
)
