from setuptools import setup, find_packages
 
setup (
    name='pygments-niels',
    description="Niels lexer for Pygments.",
    version="0.0.1",
    packages=find_packages(),
    entry_points =
    """
    [pygments.lexers]
    nielslexer = niels.lexer:NielsLexer
    """,
)

