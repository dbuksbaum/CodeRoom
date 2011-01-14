CodeRoom v0.1.5 Development

Keyboard commands:
Ctrl + S:          Save
Ctrl + W:          Save As
Ctrl + O:          Open
Ctrl + N:          New Document
Ctrl + Q:          Quit
Escape:            Quit
Ctrl + H:          Choose Syntax Highlighting
Ctrl + Return:     Create new line after the current (same as End + Return)

QuickInsert keys:
F5 to F12:         QuickInsert, as defined in syntax file
Ctrl + F5 to F12:  Define new QuickInsert on desired key

QuickInsert syntax:
First symbol:      Defined separator, sep
\n:                Newline
\t:                Tab
\s:                Space (actual spaces are used as delimiters between separators
sep:               Cursor location
sepsep:            Will be replaced by marked text, when command is pressed
sepsepsep:         Will be replaced by the current line, when command is pressed
\\n:               Plain text \n
\\t:               Plain text \t
\\s:               Plain text \s

***************************************************************************
Syntax highlighting:
***************************************************************************

Use \s as space character
Do not use \n in linespans, use restofline instead
Freewords cannot contain symbols

Priority:
multibehindregexp > multiregexp > multilinespan > singlebehindregexp > singleregexp > restofline > singlelinespan > freewords > inlinewords
The lower a span appear in the document, the higher the priority. The above still apply though.

Use // to comment. Comment must not contain COLON. Comments must have a free line or another comment above it.
