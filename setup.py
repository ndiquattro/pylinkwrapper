import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
        name="pylinkwrapper",
        version="0.0.1",
        author="Nick DiQuattro",
        author_email="nick.diquattro@betterup.co",
        description="Wrapper for using pylink in psychopy",
        long_description=long_description,
        url="https://github.com/ndiquattro/pylinkwrapper",
        packages=setuptools.find_packages())

