#!/usr/bin/env python

##
 # MIT License
 # 
 # Copyright (c) 2023 Yonder
 # 
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documentation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to whom the Software is
 # furnished to do so, subject to the following conditions:
 # 
 # The above copyright notice and this permission notice shall be included in all
 # copies or substantial portions of the Software.
 # 
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 # SOFTWARE.
 ##

from argparse import ArgumentParser, Namespace
from enum import Enum
from io import BufferedReader, BytesIO, StringIO, TextIOWrapper
from os import getenv as os_getenv, sep as ps
from pathlib import Path
from re import MULTILINE as re_MULTILINE, search as re_search, sub as re_sub
from shutil import which as shutil_which
from subprocess import PIPE as subprc_PIPE, Popen, STDOUT as subprc_STDOUT, TimeoutExpired
from sys import argv as sys_argv, exit as sys_exit, stderr
from sysconfig import get_platform as syscfg_get_platform
from typing import Iterator, Type

try:
    import yaml
except:
    raise Exception('ERROR: "PyYAML" is missing, please install "PyYAML" from pip (or install "requirements.txt")')

try:
    __file__
except NameError:
    raise Exception('ERROR: This script must be ran from a file')

if not Path(__file__).is_file():
    raise Exception('ERROR: This script must be ran from a file')

if __name__ != '__main__':
    raise Exception('ERROR: This script must be ran from a file')

platform_str: str = syscfg_get_platform().lower()
is_mingw: bool = 'mingw' in platform_str
if not is_mingw and not 'linux' in platform_str:
    raise Exception('ERROR: This script must be ran from Linux or MINGW')

dp0: Path = Path(__file__).parent.resolve()

# Fixes a Windows C:\...\...\... path into a MINGW path such as /c/.../.../...
def mingw_fixpath(path: Path) -> str:
    path_str: str = str(path)
    if is_mingw:
        if re_search(r'^[A-Z]:', path_str, flags=re_MULTILINE):
            path_str = re_sub(r'^([A-Z]):', lambda m: f'/{m.group(1).lower()}', path_str, flags=re_MULTILINE)
        return path_str.replace('\\', '/')
    else:
        return path_str

# Gather base command line to run commands on (cmd.exe runs MINGW bash, or just run bash directly on Linux)
bash: str
bash_bfx: list[str]
bash_sfx: str
if is_mingw:
    bash_path: Path = Path(r'C:\msys64\msys2_shell.cmd').resolve()
    if not bash_path.exists() or not bash_path.is_file():
        raise Exception('ERROR: Missing MINGW, please install it from https://www.msys2.org')
    else:
        bash = os_getenv('COMSPEC')
        if not bash:
            bash = 'cmd.exe'
        bash_bfx = ['CALL', str(bash_path), '-mingw64', '-defterm', '-where', str(dp0), '-no-start', '-shell', 'bash',
            '-c']
        bash_sfx = ''
else:
    bash = shutil_which('bash')
    if bash is None:
        raise Exception('ERROR: bash not found, please install it (or install "requirements_linux.txt")')
    else:
        bash = str(Path(bash).resolve())
        bash_bfx = []
        bash_sfx = f"cd '{dp0}' && "

# Gather direct path to gcc
gcc: str = shutil_which('gcc')
if gcc is None:
    raise Exception(('ERROR: gcc not found, please install it (for MINGW: "mingw-w64-x86_64-gcc") (or install "'
        '"requirements_linux.txt"/"requirements_mingw.txt")'))
else:
    gcc = mingw_fixpath(Path(gcc).resolve())

# Gather direct path to the coreutils hash command line utilities
md5sum: str = shutil_which('md5sum')
sha1sum: str = shutil_which('sha1sum')
sha256sum: str = shutil_which('sha256sum')
b2sum: str = shutil_which('b2sum')
if md5sum is None or sha1sum is None or sha256sum is None or b2sum is None:
    raise Exception(('ERROR: coreutils not found, please install it (for MINGW: "coreutils") (or install "'
        'requirements_linux.txt"/"requirements_mingw.txt")'))
else:
    md5sum = mingw_fixpath(Path(md5sum).resolve())
    sha1sum = mingw_fixpath(Path(sha1sum).resolve())
    sha256sum = mingw_fixpath(Path(sha256sum).resolve())
    b2sum = mingw_fixpath(Path(b2sum).resolve())

# Helper function to start a process
def start_process(args: list[str], timeout: int, comb_outerr: bool = None, shell: str = None) -> tuple[int, str, str]:
    proc: Popen = Popen(args=args, cwd=dp0, shell=bool(shell), executable=shell, universal_newlines=True,
        stdout=subprc_PIPE, stderr=(subprc_STDOUT if comb_outerr else subprc_PIPE))
    outs: str
    errs: str
    retc: int
    try:
        outs, errs = proc.communicate(timeout=timeout)
        retc = proc.poll()
    except TimeoutExpired:
        print(f'WARN: Process "{args[0]}" life time exceeded timeout duration {timeout:d}; killing process')
        retc = proc.poll()
        if retc is not None:
            proc.kill()
        outs, errs = proc.communicate()
    except KeyboardInterrupt:
        print(f'WARN: Process "{args[0]}" was interrupted; sending SIGINT and killing process')
        proc.send_signal(signal.SIGINT)
        retc = proc.poll()
        if retc is not None:
            proc.kill()
        outs, errs = proc.communicate()
    
    return (retc if retc else 0, outs if outs else '', errs if errs else '')

# Specific exception for validation errors (such as an argument being incorrect)
class ValidationError(ValueError):
    def __init__(self: 'ValidationError', message: str, name: str) -> None:
        super().__init__(message)
        
        self.message: str = message
        self.name: str = name
    
    def __str__(self: 'ValidationError') -> str:
        return f'Failed to validate "{self.name}": {self.message}'
    
    def __repr__(self: 'ValidationError') -> str:
        return (
            f'{self.__class__.__name__}('
            f'message={self.message!r}, '
            f'name={self.name!r})'
        )

# https://github.com/yaml/pyyaml/issues/266#issuecomment-600736794
tag_yaml_loader: yaml.SafeLoader = yaml.SafeLoader

# https://github.com/yaml/pyyaml/issues/266#issuecomment-1196644706
def construct_multi(yaml_obj_type: Type, loader: yaml.SafeLoader, node: object) -> object:
    cname: str = node.__class__.__name__
    if cname == yaml.SequenceNode.__name__:
        return yaml_obj_type(*loader.construct_sequence(node))
    elif cname == yaml.MappingNode.__name__:
        return yaml_obj_type(**loader.construct_mapping(node))
    else:
        return yaml_obj_type(loader.construct_scalar(node))

# Code list class that is loaded by PyYAML when loading the code list
class Code(yaml.YAMLObject):
    yaml_loader: yaml.SafeLoader = tag_yaml_loader
    yaml_tag: str = '!Code'
    
    def __init__(self: 'Code', file: str, name: str, author: str, description: str = None) -> None:
        self.file: str = file
        self.name: str = name
        self.author: str = author
        self.description: str = description
    
    def __repr__(self: 'Code') -> str:
        return (
            f'{self.__class__.__name__}('
            f'file={self.file!r}, '
            f'name={self.name!r}, '
            f'author={self.author!r}, '
            f'description={self.description!r})'
        )
    
    def validate(self: 'CodeList') -> bool:
        if isinstance(self.file, str):
            self.file = self.file.strip()
        
        if isinstance(self.name, str):
            self.name = self.name.strip()
        
        if isinstance(self.author, str):
            self.author = self.author.strip()
        
        if isinstance(self.description, str):
            self.description = self.description.strip()
        
        if not isinstance(self.file, str) or '\n' in self.file:
            raise ValidationError(f'Expected non-empty single line {str.__name__}', 'file')
        elif any(s in self.file for s in ':\\/"\'.%'):
            raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'file')
        
        if not isinstance(self.name, str) or '\n' in self.name:
            raise ValidationError(f'Expected non-empty single line {str.__name__}', 'name')
        elif any(s in self.name for s in ':\\/"\'.%'):
            raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'name')
        
        if not isinstance(self.author, str) or '\n' in self.author:
            raise ValidationError(f'Expected non-empty single line {str.__name__}', 'author')
        elif any(s in self.author for s in ':\\/"\'.%'):
            raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'author')
        
        if self.description is not None:
            if not isinstance(self.description, str):
                raise ValidationError(f'Expected type of {str.__name__} or {None}', 'description')
            elif any(s in self.description for s in '\\"\'%'):
                raise ValidationError('Invalid characters: \\, ", \', and/or %', 'description')
        
        return True
    
    @staticmethod
    def yaml_constructor(loader: yaml.SafeLoader, node: object) -> 'Code':
        return construct_multi(Code, loader, node)

tag_yaml_loader.add_constructor(Code.yaml_tag, Code.yaml_constructor)

# Code class that is loaded by PyYAML when loading the code list
class CodeList(yaml.YAMLObject):
    yaml_loader: yaml.SafeLoader = tag_yaml_loader
    yaml_tag: str = '!CodeList'
    
    def __init__(self: 'CodeList', project: str, title: str, author: str, game: str, game_id: str,
    codes: list[Code], assemblies: list[str] = None, global_set: str = None) -> None:
        self.project: str = project
        self.title: str = title
        self.author: str = author
        self.game: str = game
        self.game_id: str = game_id
        self.codes: list[Code] = codes
        self.assemblies: list[str] = assemblies
        self.global_set: str = global_set
    
    def __repr__(self: 'CodeList') -> str:
        return (
            f'{self.__class__.__name__}('
            f'project={self.project!r}, '
            f'title={self.title!r}, '
            f'author={self.author!r}, '
            f'game={self.game!r}, '
            f'game_id={self.game_id!r}, '
            f'codes=[{", ".join(f"{c!r}" for c in self.codes)}], '
            'assemblies = '
            f'{self.assemblies!r}' if self.assemblies is None else f'[{", ".join(f"{a!r}" for a in self.assemblies)}]'
            ')'
            f'global_set={self.global_set!r}, '
        )
    
    def validate(self: 'CodeList') -> bool:
        if isinstance(self.project, str):
            self.project = self.project.strip()
        
        if isinstance(self.title, str):
            self.title = self.title.strip()
        
        if isinstance(self.author, str):
            self.author = self.author.strip()
        
        if isinstance(self.game, str):
            self.game = self.game.strip()
        
        if isinstance(self.game_id, str):
            self.game_id = self.game_id.strip()
        
        if self.global_set is None:
            self.global_set = 'none'
        if isinstance(self.global_set, str):
            self.global_set = self.global_set.strip()
        
        if not isinstance(self.project, str) or '\n' in self.project:
            raise ValidationError(f'Expected non-empty single line {str.__name__}', 'project')
        elif any(s in self.project for s in ':\\/"\'.%'):
            raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'project')
        
        if not isinstance(self.title, str) or '\n' in self.title:
            raise ValidationError(f'Expected non-empty single line {str.__name__}', 'title')
        elif any(s in self.title for s in ':\\/"\'.%'):
            raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'title')
        
        if not isinstance(self.author, str) or '\n' in self.author:
            raise ValidationError(f'Expected non-empty single line {str.__name__}', 'author')
        elif any(s in self.author for s in ':\\/"\'.%'):
            raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'author')
        
        if not isinstance(self.game, str) or '\n' in self.game:
            raise ValidationError(f'Expected non-empty single line {str.__name__}', 'game')
        elif any(s in self.game for s in ':\\/"\'.%'):
            raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'game')
        
        if not isinstance(self.game_id, str) or '\n' in self.game_id or len(self.game_id) != 6:
            raise ValidationError(f'Expected non-empty single line {str.__name__} of length 6', 'game_id')
        elif any(s in self.game_id for s in ':\\/"\'.%'):
            raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'game_id')
        
        if not isinstance(self.codes, list) or not all(isinstance(c, Code) for c in self.codes):
            raise ValidationError(f'Expected type of {list.__name__}[{Code.__name__}]', 'codes')
        
        if self.assemblies is not None:
            if not isinstance(self.assemblies, list) or not all(isinstance(a, str) for a in self.assemblies):
                raise ValidationError(f'Expected type of {list.__name__}[{str.__name__}] or {None}', 'assemblies')
            elif self.assemblies is not None and any(s in e for s in ': \\/"\'.%-' for e in self.assemblies):
                raise ValidationError('Invalid characters: :,  , \\, /, ", \', ., and/or %', 'assemblies')
        
        # TODO: "global sets" which a codelist can specify to use global src, include, and asm folders (acts as a
        # global virtual project), defaults to none.
        # none -> no global virtual projects
        # all -> "all" global virtual project, a global project available across all projects by default
        # <name> -> /projects/global/<name>
        if self.global_set is not None:
            if not isinstance(self.global_set, str) or '\n' in self.global_set:
                raise ValidationError(f'Expected non-empty single line {str.__name__}', 'global_set')
            elif any(s in self.global_set for s in ':\\/"\'.%'):
                raise ValidationError('Invalid characters: :, \\, /, ", \', ., and/or %', 'global_set')
        
        all(c.validate() for c in self.codes)
        
        return True
    
    @staticmethod
    def yaml_constructor(loader: yaml.SafeLoader, node: object) -> 'CodeList':
        return construct_multi(CodeList, loader, node)

tag_yaml_loader.add_constructor(CodeList.yaml_tag, CodeList.yaml_constructor)

# TODO: PowerPC assembly compilation and using custom bin2c
# powerpc-eabi-as.exe -a32 -mbig -mregnames -mgekko -o /ATH/PRJ/asm/__gen__/myasm.1.o /ATH/PRJ/asm/myasm.asm
# powerpc-eabi-ld.exe -Ttext 0x80000000 -o /ATH/PRJ/asm/__gen__/myasm.2.o /ATH/PRJ/asm/__gen__/myasm.1.o
# powerpc-eabi-objcopy.exe -O binary /ATH/PRJ/asm/__gen__/myasm.2.o /ATH/PRJ/asm/__gen__/myasm.bin
# bin2c(Path(r'/ATH/PRJ/asm/__gen__/myasm.bin'), bin2cSize.UINT32, Path(r'/ATH/PRJ/src/__gen__/myasm.c'), Path(r'/ATH/PRJ/include/__gen__/myasm.h'))
# #include <__gen__/myppc_example.h>
# bin2c - Binary data to C array declarations
class bin2cSize(Enum):
    UINT8 = 1
    UINT16 = 2
    UINT32 = 4

def bin2c(bin_path: Path, nlen: bin2cSize, srcdir_path: Path, base_incldir_path: Path = None,
incldir_path: Path = None) -> None:
    br = BufferedReader
    with bin_path.open(mode='rb') as br:
        br.seek(0, os.SEEK_END)
        size: int = br.tell()
        if size > 0xFFFFFFFF / nlen.value:
            maxszidx: int = 0
            match nlen:
                case bin2cSize.UINT8:
                    maxszidx = 4
                case bin2cSize.UINT16:
                    maxszidx = 2
                case bin2cSize.UINT32:
                    maxszidx = 1
            raise Exception(f'Max {maxszidx:d} GiB supported')
        br.seek(0, os.SEEK_SET)
        
        bin_name: str = bin_path.name
        all(bin_name := bin_name.replace(s, '_') for s in ': \\/"\'.%-' if s in bin_name)
        
        gen_h: bool = base_incldir_path and incldir_path
        
        if not srcdir_path.exists():
            raise Exception(f'ERROR: "{srcdir_path}" does not exist')
        elif srcdir_path.is_file():
            raise Exception(f'ERROR: "{srcdir_path}" is not a directory')
        
        src_path: Path = srcdir_path.joinpath(f'{bin_name}.c').resolve()
        if src_path.exists() and not src_path.is_file():
            raise Exception(f'ERROR: "{src_path}" already exists as a directory')
        
        if gen_h:
            if not incldir_path.exists():
                raise Exception(f'ERROR: "{incldir_path}" does not exist')
            elif incldir_path.exists() and incldir_path.is_file():
                raise Exception(f'ERROR: "{incldir_path}" is not a directory')
            
            incl_path: Path = incldir_path.joinpath(f'{bin_name}.h').resolve()
            if incl_path.exists() and not incl_path.is_file():
                raise Exception(f'ERROR: "{incl_path}" already exists as a directory')
        
        data_name: str = f'uint{nlen.value * 8:d}_t *{bin_name}_data'
        length_name: str = f'int32_t {bin_name}_length'
        
        cw: TextIOWrapper
        with src_path.open(mode='wt', encoding='utf_8', newline='\n') as cw:
            cw.write('// Generated by bin2c, do not modify.\n')
            
            if gen_h:
                cw.write('\n#include <')
                cw.write(f'{incl_path.relative_to(base_incldir_path)}')
                cw.write('>\n\n')
            else:
                cw.write('\n#include <stdint.h>\n\n')
            
            cw.write(data_name)
            cw.write(' = { ')
            
            i: int = 0
            ba: bytes
            while ba := br.read(1024 * 64):
                bi: BytesIO
                with BytesIO(ba) as bi:
                    bb: bytes
                    while bb := bi.read(nlen.value):
                        if i % nlen.value == 0:
                            cw.write('0x')
                        cw.write(f'{bb.hex():0>{nlen.value * 2}}')
                        i += nlen.value
                        if i < size:
                            cw.write(', ')
            
            cw.write(' };\n\n')
            cw.write(length_name)
            cw.write(' = ')
            cw.write(f'{floor(i / nlen.value):d}')
            cw.write(';\n')
    
    if gen_h:
        hw: TextIOWrapper
        with incl_path.open(mode='wt', encoding='utf_8', newline='\n') as hw:
            hw.write('// Generated by bin2c, do not modify.\n')
            
            bin_name_upper: str = bin_name.to_upper()
            hw.write(f'\n#ifndef __{bin_name_upper}_H__\n#define __{bin_name_upper}_H__')
            
            cw.write('\n#include <stdint.h>\n\nextern ')
            cw.write(data_name)
            cw.write(';\n\nextern ')
            cw.write(length_name)
            cw.write(';\n#endif')

def argparse_anyint(s: str):
    return int(s, 0)

# Script entry point
def main(argv: list[str]) -> int:
    parser: ArgumentParser = ArgumentParser(prog='compile.py',
        description=('Compiles a code list for use with gecko. This will find the code list via the path (based on '
            f'passed arguments): {ps}projects{ps}<author>{ps}<project>{ps}codelist.yaml'))
    parser.add_argument('author', metavar='author', type=str,
        help='The author of the project. The target code list should also specify this same author')
    parser.add_argument('project', metavar='project', type=str,
        help='The project name. The target code list should also specify this same project.')
    parser.add_argument('-a', '--address', required=False, type=argparse_anyint, default=0x80001800,
        help='Address of the code handler. Required for gecko.h addresses to be correct. Defaults to: 0x80001800')
    parser.add_argument('-s', '--size', required=False, type=argparse_anyint, default=2880,
        help=('Size of the code handler in bytes. Required for some extra functionality in gecko.h to be work. This '
            'has no effect if the compat option not passed. Defaults to: 2880'))
    parser.add_argument('-c', '--compat', action='store_true', required=False, default=False,
        help=('Compile gecko.h for compatibility with codehandler.s (Dolphin, Nintendont, etc.). Enables extra '
            'features in gecko.h.'))
    parser.add_argument('-d', '--debug', action='store_true', required=False, default=False,
        help='Compile as debug. Also, for Linux only: include address sanitizer (ASAN), stack protector, etc.')
    args: Namespace = parser.parse_args(argv[1:])
    if (args.project[0] == '"' and args.project[-1] == '"') or (args.project[0] == "'" and args.project[-1] == "'"):
        args.project = args.project[1:-1]
    
    # Validate code list input
    code_list_file: Path = dp0.joinpath('projects', args.author, args.project, 'codelist.yaml')
    code_list_file = code_list_file.resolve()
    if not code_list_file.exists():
        print(f'ERROR: "{code_list_file}" does not exist', file=stderr)
        return 1
    elif not code_list_file.is_file():
        print(f'ERROR: "{code_list_file}" is not a file', file=stderr)
        return 1
    
    # Read input code list and validate it
    code_list: CodeList = None
    try:
        print(f'Reading code list "{code_list_file}"')
        code_list_io: TextIOWrapper
        with code_list_file.open(mode='rt', encoding='utf_8') as code_list_io:
            code_list = yaml.load(code_list_io, Loader=tag_yaml_loader)
        code_list.validate()
    except ValidationError as e:
        print(f'ERROR: Validation failed for the code list "{code_list_file}": {e}', file=stderr)
        return 1
    except yaml.YAMLError as e:
        print(f'ERROR: PyYAML failed to parse the code list "{code_list_file}": {e}', file=stderr)
        return 1
    
    if code_list.author != args.author:
        print(f'ERROR: Code list author "{code_list.author}" does not match the provided author "{args.author}"',
            file=stderr)
        return 1
    
    if code_list.project != args.project:
        print(f'ERROR: Code list project "{code_list.project}" does not match the provided project "{args.project}"',
            file=stderr)
        return 1
    
    # Code list directory
    code_lists_dir: Path = dp0.joinpath('projects', code_list.author, code_list.project).resolve()
    if not code_lists_dir.exists():
        print(f'ERROR: "{code_lists_dir}" does not exist', file=stderr)
        return 1
    elif not code_lists_dir.is_dir():
        print(f'ERROR: "{code_lists_dir}" is not a directory', file=stderr)
        return 1
    
    code_lists_srcdir: Path = code_lists_dir.joinpath('src').resolve()
    if not code_lists_srcdir.exists():
        print(f'ERROR: "{code_lists_srcdir}" does not exist', file=stderr)
        return 1
    elif not code_lists_srcdir.is_dir():
        print(f'ERROR: "{code_lists_srcdir}" is not a directory', file=stderr)
        return 1
    
    # Get associated source code files for every code in the code list
    code_files: list[str] = []
    code: Code
    for code in code_list.codes:
        print(f'Code list specified source file name: "{code.file}"')
        
        code_file: Path = None
        new_code_files: Iterator[Path] = code_lists_srcdir.rglob(f'{code.file}.c')
        new_code_file: Path
        for new_code_file in new_code_files:
            if code_file:
                print(f'ERROR: Duplicate source files for "{code.file}.c"', file=stderr)
                return 1
            
            if not new_code_file.is_file():
                print(f'ERROR: "{new_code_file}" is not a file', file=stderr)
                return 1
            
            code_file = new_code_file.resolve()
        
        if not code_file:
            print(f'ERROR: "{code.file}.c" does not exist anywhere under "{code_lists_srcdir}"', file=stderr)
            return 1
        
        print(f'Found source file for code list: "{code_file}"')
        code_files.append(f"'{mingw_fixpath(code_file)}'")
    
    # Code generation directory
    code_gen_dir: Path = dp0.joinpath('include', '__gen__').resolve()
    if code_gen_dir.exists():
        if not code_gen_dir.is_dir():
            print(f'ERROR: "{code_gen_dir}" already exists as a file', file=stderr)
            return 1
    else:
        print(f'Creating "{code_gen_dir}"')
        code_gen_dir.mkdir(parents=True)
    
    GECKO_MIT_LICENSE: str = (
   '/*'
 '\n * MIT License'
 '\n * '
 '\n * Copyright (c) 2023 Yonder'
 '\n * '
 '\n * Permission is hereby granted, free of charge, to any person obtaining a copy'
 '\n * of this software and associated documentation files (the "Software"), to deal'
 '\n * in the Software without restriction, including without limitation the rights'
 '\n * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell'
 '\n * copies of the Software, and to permit persons to whom the Software is'
 '\n * furnished to do so, subject to the following conditions:'
 '\n * '
 '\n * The above copyright notice and this permission notice shall be included in all'
 '\n * copies or substantial portions of the Software.'
 '\n * '
 '\n * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR'
 '\n * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,'
 '\n * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE'
 '\n * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER'
 '\n * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,'
 '\n * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE'
 '\n * SOFTWARE.'
 '\n */'
    )
    
    # Generate __gen__/standard.h which is what standard.h depends on
    standard_gen_file: Path = code_gen_dir.joinpath('standard.h').resolve()
    standard_gen_io: TextIOWrapper
    with standard_gen_file.open(mode='wt', encoding='utf_8', newline='\n') as standard_gen_io:
        print(f'Generating "{standard_gen_file}"')
        
        # Source file beginning, code list format enum, code list project and title name, print code list function,
        # and code list header
        standard_gen_io.write((
 f'{GECKO_MIT_LICENSE}'
 '\n'
 '\n// Generated code output file from compile.py, do not modify.'
 '\n'
 '\n#ifndef __GEN_STANDARD_H__'
 '\n#define __GEN_STANDARD_H__'
 '\n#include <__gen__/standard_defs.h>'
 '\n#include <stdext/cmacros.h>'
 '\n'
 '\n#define __STANDARD_USAGE_INTRO__ \\'
f'\n    "{code_list.title} by {code_list.author}\\n" \\'
f'\n    "USAGE: {code_list.project}"'
 '\n'
 '\ntypedef enum __CLFFmt {'
 '\n    CLF_DOLPHIN,'
 '\n    CLF_GCT,'
 '\n    CLF_OCARINA,'
 '\n    CLF_RAW,'
 '\n    CLF_RAWTEXT,'
 '\n    CLF_NONE = 0xFF'
 '\n} CLFFmt;'
 '\n'
 '\nINLINE void printclf(CLFFmt lfmt) {'
 '\n    if (lfmt == CLF_DOLPHIN)'
f'\n        fprintf(G_OutputHandle, "; {code_list.title} by {code_list.author}\\n[Gecko]\\n");'
 '\n    else if (lfmt == CLF_GCT)'
 '\n        G_BeginGCT();'
 '\n    else if (lfmt == CLF_OCARINA)'
 '\n        fprintf(G_OutputHandle,'
f'\n            "{code_list.game_id}\\n{code_list.game}\\n\\n"'
f'\n            "{code_list.title} by {code_list.author}\\n\\n"'
 '\n        );'
 '\n    '
        ))
        
        # Code list codes
        code: Code
        for code in code_list.codes:
            # Code header (name, author)
            standard_gen_io.write((
 '\n    if (lfmt == CLF_DOLPHIN)'
f'\n        fprintf(G_OutputHandle, "${code.name} [{code.author}]\\n");'
 '\n    else if (lfmt == CLF_OCARINA)'
f'\n        fprintf(G_OutputHandle, "{code.name} [{code.author}]\\n");'
f'\n    {code.file}();'
            ))
            
            # Code description
            if code.description:
                standard_gen_io.write((
 '\n    if (lfmt == CLF_DOLPHIN) {'
 '\n        fprintf(G_OutputHandle, ('
                ))
                
                desc_line: str
                for desc_line in StringIO(code.description):
                    standard_gen_io.write((
f'\n            "*{desc_line.strip()}\\n"'
                    ))
                
                standard_gen_io.write((
 '\n        ));'
 '\n    } else if (lfmt == CLF_OCARINA) {'
 '\n        fprintf(G_OutputHandle, ('
                ))
                
                desc_line: str
                for desc_line in StringIO(code.description):
                    standard_gen_io.write((
f'\n            "{desc_line.strip()}\\n"'
                    ))
                
                standard_gen_io.write((
 '\n            "\\n"'
 '\n        ));'
 '\n    }'
                ))
            
            standard_gen_io.write((
 '\n    '
            ))
        
        # Code footer and source file ending
        standard_gen_io.write((
 '\n    if (lfmt == CLF_GCT)'
 '\n        G_EndGCT();'
 '\n}'
 '\n#endif\n'
        ))
    
    # Generate __gen__/standard_defs.h which is what the source for codes and __gen__/standard.h depend on
    standard_defs_gen_file: Path = code_gen_dir.joinpath('standard_defs.h').resolve()
    standard_defs_gen_io: TextIOWrapper
    with standard_defs_gen_file.open(mode='wt', encoding='utf_8', newline='\n') as standard_defs_gen_io:
        print(f'Generating "{standard_defs_gen_file}"')
        
        # Source file beginning
        standard_defs_gen_io.write((
 f'{GECKO_MIT_LICENSE}'
 '\n'
 '\n// Generated code output file from compile.py, do not modify.'
 '\n'
 '\n#ifndef __GEN_STANDARD_DEFS_H__'
 '\n#define __GEN_STANDARD_DEFS_H__'
 '\n#include <stdio.h>'
 '\n#include <stdint.h>'
 '\n#include <gecko.h>'
 '\n'
        ))
        
        # Each code in codelist generates a function declaration for use by code source files and __gen__/standard.h
        code: Code
        for code in code_list.codes:
            standard_defs_gen_io.write((
f'\nvoid {code.file}(void);'
            ))
        
        # Source file ending
        standard_defs_gen_io.write((
'\n#endif\n'
        ))
    
    GECKO_MIT_LICENSE = None
    
    # Start constructing gcc command arguments
    gcc_cmd: list[str] = ['-std=gnu99']
    
    # Output directory
    bin_dir: Path = dp0.joinpath('bin', code_list.author).resolve()
    if bin_dir.exists():
        if not bin_dir.is_dir():
            print(f'ERROR: "{bin_dir}" already exists as a file', file=stderr)
            return 1
    else:
        print(f'Creating "{bin_dir}"')
        bin_dir.mkdir(parents=True)
    
    # Output file
    out_name: str = f'{code_list.project}'
    if is_mingw:
        out_name += '.exe'
    
    out_file: Path = bin_dir.joinpath(out_name).resolve()
    if out_file.exists():
        if not out_file.is_file():
            print(f'ERROR: "{out_file}" already exists as a directory', file=stderr)
            return 1
        else:
            out_file.unlink()
    gcc_cmd.append(f"-o '{mingw_fixpath(out_file)}'")
    
    # Optimization and debug flags
    if args.debug:
        gcc_cmd.append('-O0')
        gcc_cmd.append('-g3')
        gcc_cmd.append('-ggdb')
        if not is_mingw:
            gcc_cmd.append('-fsanitize=address')
            gcc_cmd.append('-fsanitize=undefined')
            gcc_cmd.append('-fno-sanitize-recover')
            gcc_cmd.append('-fstack-protector-strong')
    else:
        gcc_cmd.append('-O1')
    
    # Warning and error flags
    if args.debug:
        gcc_cmd.append('-Wall')
        gcc_cmd.append('-Wno-unused-function')
    gcc_cmd.append('-Wno-unused-but-set-variable')
    gcc_cmd.append('-Wno-unused-variable')
    gcc_cmd.append('-Werror=vla')
    gcc_cmd.append('-Werror=missing-prototypes')
    
    # Preprocessor definition flags
    gcc_cmd.append(f'-D __GECKO_H_CODEHANDLERADDR__=0x{args.address:08X}')
    gcc_cmd.append('-D __STDEXT_CMACROS_H_DEPDEFS__')
    if args.compat:
        gcc_cmd.append(f'-D __GECKO_H_CODEHANDLERSIZE__={args.size:d}')
        gcc_cmd.append('-D __GECKO_H_CODEHANDLERSCOMPAT__')
    if args.debug:
        gcc_cmd.append('-D __STDEXT_INCLERRSTRGS__')
    
    # Include directories
    include_dir: Path = dp0.joinpath('include').resolve()
    if not include_dir.exists():
        print(f'ERROR: "{include_dir}" does not exist', file=stderr)
        return 1
    elif not include_dir.is_dir():
        print(f'ERROR: "{include_dir}" is not a directory', file=stderr)
        return 1
    
    gcc_cmd.append(f"-I'{mingw_fixpath(include_dir)}'")
    extra_include_dirs: Iterator[Path] = include_dir.rglob('*')
    if extra_include_dirs:
        extra_include_dir: Path
        for extra_include_dir in extra_include_dirs:
            if extra_include_dir.is_dir():
                gcc_cmd.append(f"-I'{mingw_fixpath(extra_include_dir.resolve())}'")
    
    code_lists_incldir: Path = code_lists_dir.joinpath('include').resolve()
    if not code_lists_incldir.exists():
        print(f'ERROR: "{code_lists_incldir}" does not exist', file=stderr)
        return 1
    elif not code_lists_incldir.is_dir():
        print(f'ERROR: "{code_lists_incldir}" is not a directory', file=stderr)
        return 1
    
    gcc_cmd.append(f"-I'{mingw_fixpath(code_lists_incldir)}'")
    cl_extra_incl_dirs: Iterator[Path] = code_lists_incldir.rglob('*')
    if cl_extra_incl_dirs:
        cl_extra_incl_dir: Path
        for cl_extra_incl_dir in cl_extra_incl_dirs:
            if cl_extra_incl_dir.is_dir():
                gcc_cmd.append(f"-I'{mingw_fixpath(cl_extra_incl_dir.resolve())}'")
    
    # Source files
    sources_dir: Path = dp0.joinpath('src').resolve()
    if not sources_dir.exists():
        print(f'ERROR: "{sources_dir}" does not exist', file=stderr)
        return 1
    elif not sources_dir.is_dir():
        print(f'ERROR: "{sources_dir}" is not a directory', file=stderr)
        return 1
    
    source_files: Iterator[Path] = sources_dir.rglob('*.c')
    has_sources: int = 0
    if source_files:
        source_file: Path
        for source_file in source_files:
            if source_file.is_file():
                has_sources += 1
                gcc_cmd.append(f"'{mingw_fixpath(source_file.resolve())}'")
    
    code_file: str
    for code_file in code_files:
        has_sources += 1
        gcc_cmd.append(code_file)
    
    if has_sources < 2:
        print(f'ERROR: Some source files are missing', file=stderr)
        return 1
    
    # Start compiling stuff
    print(f'Compiling "{out_file}"', file=stderr)
    
    # Create compile log file
    log_file: Path = bin_dir.joinpath(f'{out_name}.log').resolve()
    if log_file.exists() and not log_file.is_file():
        print(f'ERROR: "{log_file}" already exists as a directory', file=stderr)
        return 1
    
    # Compile
    log_io: TextIOWrapper
    with log_file.open(mode='wt', encoding='utf_8', newline='\n') as log_io:
        gcc_cmd_s: str = r' '.join(gcc_cmd)
        log_io.write(start_process([*bash_bfx, (
f"{bash_sfx}echo '{gcc} --version' && {gcc} --version && echo '{gcc} {gcc_cmd_s}' && {gcc} {gcc_cmd_s}"
        )], 60, True, bash)[1])
    
    # Start hash stuff
    if out_file.exists():
        print(f'Computing hashes of "{out_file}"')
        
        md5hash_file: Path = bin_dir.joinpath(f'{out_name}.md5').resolve()
        if md5hash_file.exists() and not md5hash_file.is_file():
            print(f'ERROR: "{md5hash_file}" already exists as a directory', file=stderr)
            return 1
        
        sha1hash_file: Path = bin_dir.joinpath(f'{out_name}.sha1').resolve()
        if sha1hash_file.exists() and not sha1hash_file.is_file():
            print(f'ERROR: "{sha1hash_file}" already exists as a directory', file=stderr)
            return 1
        
        sha256hash_file: Path = bin_dir.joinpath(f'{out_name}.sha256').resolve()
        if sha256hash_file.exists() and not sha256hash_file.is_file():
            print(f'ERROR: "{sha256hash_file}" already exists as a directory', file=stderr)
            return 1
        
        b2hash_file: Path = bin_dir.joinpath(f'{out_name}.blake2').resolve()
        if b2hash_file.exists() and not b2hash_file.is_file():
            print(f'ERROR: "{b2hash_file}" already exists as a directory', file=stderr)
            return 1
        
        outx: str = mingw_fixpath(out_file)
        
        # Compute md5 hash
        md5hash_io: TextIOWrapper
        with md5hash_file.open(mode='wt', encoding='utf_8', newline='\n') as md5hash_io:
            md5hash_io.write(start_process([*bash_bfx, (
f"{bash_sfx}{md5sum} -b '{outx}'"
            )], 30, True,bash)[1])
        
        # Compute sha1 hash
        sha1hash_io: TextIOWrapper
        with sha1hash_file.open(mode='wt', encoding='utf_8', newline='\n') as sha1hash_io:
            sha1hash_io.write(start_process([*bash_bfx, (
f"{bash_sfx}{sha1sum} -b '{outx}'"
            )], 30, True, bash)[1])
        
        # Compute sha256 hash
        sha256hash_io: TextIOWrapper
        with sha256hash_file.open(mode='wt', encoding='utf_8', newline='\n') as sha256hash_io:
            sha256hash_io.write(start_process([*bash_bfx, (
f"{bash_sfx}{sha256sum} -b '{outx}'"
            )], 30, True, bash)[1])
        
        # Compute b2 hash
        b2hash_io: TextIOWrapper
        with b2hash_file.open(mode='wt', encoding='utf_8', newline='\n') as b2hash_io:
            b2hash_io.write(start_process([*bash_bfx, (
f"{bash_sfx}{b2sum} -b '{outx}'"
            )], 30, True, bash)[1])
        
        print('Finished')
        return 0
    else:
        print(f'There was an error compiling, please check "{log_file}"')
        return 1

# Start script (run into entry point)
sys_exit(main(sys_argv))
