from setuptools import setup

setup(
    name='reapython',
    version='1.0.2',
    author='robbeykaaso',
    author_email='robbey_kaaso@sina.com',
    url='https://rea2.readthedocs.io/en/latest/',
    description='rea.py',
    packages=['reapython'],
    install_requires=['PyQt5'],
    entry_points={
        'console_scripts': [
            'help=reapython:help'
        ]
    }
)