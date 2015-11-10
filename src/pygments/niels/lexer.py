# -*- coding: utf-8 -*-
"""
    pygments.lexers.niels
    ~~~~~~~~~~~~~~~~~~~~~~

    Lexer for the Niels language.

    :copyright: Copyright 2006-2015 by the Pygments team, see AUTHORS.
    :license: BSD, see LICENSE for details.
"""

from pygments.lexer import RegexLexer, bygroups
from pygments.token import Text, Comment, Keyword, Number, String, Name, Operator

__all__ = ['NielsLexer']

class NielsLexer(RegexLexer):
    """
    For `Niels <http://www.bh107.org/>`_ source.

    """
    name = 'Niels'
    aliases = ['niels', 'nls']
    filenames = ['*.nls']

    tokens = {
        'root': [
            (r'\n', Text),
            (r'\s+', Text),
            (r'\\\n', Text),
            (r"(\+)", Operator),
            (r"(\-)", Operator),
            (r"(\*)", Operator),
            (r"(%)", Operator),
            (r"\?", Operator),
            (r">", Operator),
            (r"<", Operator),
            (r"@", Operator),
            (r"\^", Operator),
            (r"<=", Operator),
            (r"(>=)", Operator),
            (r"(==)", Operator),
            (r"(!=)", Operator),
            (r"(and|or|xor|not|bw\(and\)|bw\(or\)|bw\(xor\)|bw\(not\)|lshift|rshift)", Operator),
            (r"\".*\"", String),
            (r"'.*'", String),
            (r"\{", Text),
            (r"\}", Text),
            (r"\(", Text),
            (r"\)", Text),
            (r"\[", Text),
            (r"\]", Text),
            (r",", Text),
            (r":", Text),
            (r";", Text),
            (r"(int|i32|i64|r32|r64|real|true|false|bool)", Keyword.Type),
            (r"[0-9]+\.[0-9]+", Number.Float),
            (r"[0-9]+", Number.Integer),
            (r'(import|while|when|is|as|otherwise)\b', Keyword),
            (r'(function|record|return)\b', Keyword),
            (r"[a-zA-Z][a-zA-Z0-9]?", Name),
            (r'/\*', Comment.Multiline, 'comment'),
        ],
        'comment': [
            (r'[^*/]', Comment.Multiline),
            (r'/\*', Comment.Multiline, '#push'),
            (r'\*/', Comment.Multiline, '#pop'),
            (r'[*/]', Comment.Multiline)
        ]
    }

