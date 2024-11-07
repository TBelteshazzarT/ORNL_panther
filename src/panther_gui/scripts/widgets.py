#!/usr/bin/env python3
# List View Class
import tkinter

import customtkinter


class ListView(customtkinter.CTkScrollableFrame):
    def __init__(self, master, element_generator, size=0, command=None, *args, **kwargs):
        super().__init__(master, *args, **kwargs)
        self._master = master
        self._element_generator = element_generator
        self._command = command
        self._elements = []
        self.resize(size)

    def resize(self, size):
        if size < len(self._elements):
            for element in self._elements[size:]:
                element.destroy()
            del self._elements[size:]
        else:
            for i in range(size - len(self._elements)):
                self._elements.append(self._element_generator(self, self._command, ''))
                self._elements[-1].pack(fill=customtkinter.X)

    def get_values(self):
        return [element.get() for element in self._elements]

    def cget_values(self, attribute_name: str):
        return [element.cget(attribute_name=attribute_name) for element in self._elements]

    def remove(self, index):
        self._elements[index].destroy()
        self._elements.pop(index)

    def set_values(self, value):
        self.resize(0)
        for i in value:
            element = self._element_generator(self, self._command, *i)
            element.pack(fill=customtkinter.X)
            self._elements.append(element)

    def add_value(self, value):
        element = self._element_generator(self, self._command, value)
        element.pack(fill=customtkinter.X)
        self._elements.append(element)

    def __getitem__(self, key: int):
        return self._elements[key]

    def __len__(self):
        return len(self._elements)

    def __iter__(self):
        return (self._elements[i] for i in range(len(self._elements)))


# Integer Input Class
class IntegerInput(customtkinter.CTkEntry):
    def __init__(self, master, command=None, default_value='', max_value=None, **kwargs):
        self._var = tkinter.StringVar(master)
        self._var.set(default_value)
        if command is not None:
            self._var.trace_add('write', lambda *args: self._enforce_and_update(*args))
        super().__init__(master, textvariable=self._var, **kwargs)
        self._max_value = max_value
        self._command = command

    def get_string_var(self):
        return self._var

    def set_max_value(self, max_value=None):
        if self._max_value != max_value:
            self._max_value = max_value
            self._enforce_integer()

    def get(self):
        value = self._var.get()
        return int(value) if len(value) > 0 else 0

    def set(self, value=''):
        self._var.set(value)

    def _enforce_and_update(self, *args):
        self._enforce_integer(*args)
        self._command(*args)

    def _enforce_integer(self, *args):
        value = ''.join([element if element.isdigit() else '' for element in self._var.get()])
        if self._max_value is not None:
            value = str(min(int(value) if value != '' else 0, self._max_value))
        value = value if value != '0' else ''
        if value != self._var.get():
            self._var.set(value)

    @property
    def entry(self):
        return self._entry

# Number Entry
class NumberInput(customtkinter.CTkEntry):
    def __init__(self, master, command=None, default_value='', max_value=None, **kwargs):
        self._var = tkinter.StringVar(master)
        self._var.set(default_value)
        if command is not None:
            self._var.trace_add('write', lambda *args: self._enforce_and_update(*args))
        super().__init__(master, textvariable=self._var, **kwargs)
        self._max_value = max_value
        self._command = command

    def get_string_var(self):
        return self._var

    def set_max_value(self, max_value=None):
        if self._max_value != max_value:
            self._max_value = max_value
            self._enforce_number()

    def get(self):
        value = self._var.get()
        return float(value) if len(value) > 0 else 0

    def set(self, value=''):
        self._var.set(value)

    def _enforce_and_update(self, *args):
        self._enforce_number(*args)
        self._command(*args)

    def _enforce_number(self, *args):
        value = ''.join([element if element in ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'] else '' for element in self._var.get()])
        if self._max_value is not None:
            value = str(min(float(value) if value != '' else 0, self._max_value))
        value = value if value != '0' else ''
        if value != self._var.get():
            self._var.set(value)

    @property
    def entry(self):
        return self._entry


# Scrollable Table View Class
class ScrollableTableView(customtkinter.CTkScrollableFrame):
    def __init__(self, master, values, *args, **kwargs):
        super().__init__(master, orientation=customtkinter.HORIZONTAL, *args, **kwargs)
        self._values = values
        self._elements = []
        self._font = None
        self._build_table(values)

    def _build_table(self, values):
        # Pre-build the font for measuring
        if self._font is None:
            self._font = tkinter.font.Font()

        # Match array shape | Rows
        while len(self._elements) > len(values):
            for element in self._elements[-1]:
                element.destroy()
            self._elements.pop()
        while len(self._elements) < len(values):
            self._elements.append([])
        # Match array shape | Columns
        for row in range(len(self._elements)):
            while len(self._elements[row]) > len(values[row]):
                self._elements[row][-1].destroy()
                self._elements[row].pop()
            while len(self._elements[row]) < len(values[row]):
                self._elements[row].append(0)

        # Change text in existing elements or change elements to labels
        for i in range(len(values)):
            for j in range(len(values[i])):
                if type(self._elements[i][j]) is customtkinter.CTkLabel:
                    self._elements[i][j].configure(text=values[i][j], width=self._font.measure(values[i][j]))
                else:
                    self._elements[i][j] = customtkinter.CTkLabel(self, text=values[i][j],
                                                                  fg_color=('gray75', 'gray25'),
                                                                  width=self._font.measure(values[i][j]))
                    self._elements[i][j].grid(row=i, column=j, padx=1, pady=1, sticky=customtkinter.NSEW)

        # Replace values
        self._values = values

    def set_values(self, values):
        self._build_table(values)

    def set_column(self, index, values):
        for row in self._values:
            if len(row) == index:
                row.append(values.pop(0))
            else:
                row[index] = values.pop(0)
        self._build_table(self._values)

    def __setitem__(self, key: tuple, value):
        self._elements[key[0]][key[1]] = value

    def __getitem__(self, key: tuple):
        return self._elements[key[0]][key[1]]

    def get_values(self) -> list[list[str]]:
        return self._values


# Copy Label
class CopyableLabel(customtkinter.CTkLabel):
    def __init__(self, master, *args, **kwargs):
        super().__init__(master, *args, **kwargs)
        self.bind('<Button-1>', self._put_text_in_clipboard)

    def _put_text_in_clipboard(self, *args):
        self.clipboard_clear()
        self.clipboard_append(self.cget('text'))