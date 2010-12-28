CodeRoom v0.1 Alpha

Keyboard commands:
Ctrl + S: Save
Ctrl + W: Save As
Ctrl + O: Open
Ctrl + N: New
Ctrl + Q: Quit


In syntax:

Words, etc, implemented as:

QRegExp("\\bWORD\\b")

Rest of line expressions:

QRegExp("START[^\n]*");

Multiline, or separated expressions:

QRegExp("START");
QRegExp("END");

Translations:

/   /
\   \\\\
*   \\*
^   \\^
+   \\+
-   \\-
?   \\?
{   \\{
}   \\}
=   \\=
!   \\!
(   \\(
)   \\)
[   \\[
]   \\]
"   \"
'   \'
