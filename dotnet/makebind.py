#!/usr/bin/env python

__author__ = 'john'

import argparse
from pyparsing import Keyword, Word, alphas, ZeroOrMore, delimitedList
from pyparsing import Group, Optional, ParseException, Literal, cStyleComment

# Code generators
class CSharpGen:
    def emit(self, code):
        lines = []
        lines.append('using System;')
        lines.append('using System.Runtime.InteropServices;')
        lines.append('\n')
        lines.append('public static class __X {')
        for cl in code:
            if cl[0] == 'DLL_PUBLIC':
                lines.append(self.emitFunction(cl))
            elif cl[0] == 'typedef':
                lines.append(self.emitDelegate(cl))
            elif cl[0] == 'enum':
                lines.append(self.emitEnum(cl))
        lines.append('}')
        return lines

    def emitEnum(self, cl):
        line = ['public enum ', cl[1], ' {']
        body = []
        for val in cl[3]:
            body.append(val)

        return '\t'+''.join(line) + '\n\t\t'+',\n\t\t'.join(body) + '\n\t}'

    def emitDelegate(self, cl):
        code = ['\tpublic delegate']
        code.append(self.emitReturn(cl))
        code.append(cl[4])
        code.append('('+self.emitFunctionArguments(cl[7:-1])+');')
        return ' '.join(code)

    def emitFunctionArgument(self, a):
        arg = ' '.join(a)
        if arg.find('const struct') != -1:
            return ' '.join(['System.IntPtr', a[4]])
        if arg.find('struct') != -1:
            return ' '.join(['System.IntPtr', a[3]])
        elif a[0] == 'unsigned':
            return ' '.join(['uint', a[2]])
        elif a[0] == 'enum':
            return ' '.join(a[1:])
        elif a[0] == 'size_t':
            return ' '.join(['uint', a[1]])
        elif len(a) == 1:
            return ''
        elif ' '.join(a[:2]) == 'void *':
            return ' '.join(['System.IntPtr', a[2]])
        else:
            return ' '.join(a)

    def emitFunctionArguments(self, args):
        code = []
        for arg in args:
            code.append(self.emitFunctionArgument(arg))
        return ', '.join(code)

    def emitReturn(self, ret):
        if ret[1][0] == 'struct':
            return 'System.IntPtr'
        elif ret[1][0] == 'size_t':
            return 'uint'
        else:
            return ret[1][0]

    def emitFunction(self, cl):
        code1 = '\t[DllImport("revolution")]'
        code2 = '\tpublic extern static ' + self.emitReturn(cl) + ' '\
              + cl[2] + '(' + self.emitFunctionArguments(cl[4:-1]) +');'
        return '\n'.join([code1, code2])

CodeGenerators = { 'csharp' : CSharpGen() }
GENERATOR = None

# common keywords: int, double, unsigned int, void
voidKwd = Keyword('void')
constKwd= Keyword('const')
intKwd  = Keyword('int')
doubleKwd = Keyword('double')
unsignedIntKwd = Keyword('unsigned') + intKwd
sizetKwd = Keyword('size_t')
commonKwd = voidKwd | unsignedIntKwd | intKwd | doubleKwd | sizetKwd
structKwd = Keyword('struct')
enumKwd   = Keyword('enum')
typedefKwd= Keyword('typedef')
pointerSymbol = Literal('*')

# fun declaration = return_type fun_name '(' args list ');'
# case 1. struct Struct_name *
# case 2. enum Enum_name
# case 3. int, double, void, unsigned int
funReturnType = structKwd + Word(alphas) + pointerSymbol \
              | enumKwd + Word(alphas) \
              | commonKwd

# fun argument
# case 1. struct Struct_name * Argument_name
# case 2. enum Enum_name Argument_name
# case 3. int Argument_name, void ** Argument_name
# case 4. Some_typedef_name Argument_name
# case 5. void
argumentName   = Word(alphas).setResultsName('argumentName')
typedefName    = Word(alphas)
enumName       = Word(alphas)
structName     = Word(alphas)
structArgument = structKwd + structName + pointerSymbol + argumentName
commonArgument = commonKwd + ZeroOrMore(pointerSymbol) + argumentName
constCommonArgument = constKwd + commonArgument
funArgument = \
              structArgument \
            | constKwd + structArgument \
            | enumKwd + enumName + argumentName \
            | constCommonArgument \
            | commonArgument \
            | typedefName + argumentName \
            | voidKwd

funName = Word(alphas)
argumentList = Optional(delimitedList(Group(funArgument)))
# function declaration
function = Group(funReturnType).setResultsName('returnType') \
         + funName.setResultsName('functionName') \
         + '(' \
         + argumentList.setResultsName('arguments') \
         + ');'
function.ignore(cStyleComment)

# function typedef
funTypedefName = funName
funTypedef = typedefKwd \
           + Group(funReturnType).setResultsName('returnType') \
           + '(' \
           + pointerSymbol \
           + funTypedefName.setResultsName('functionName') \
           + ')' \
           + '(' \
           + argumentList.setResultsName('arguments') \
           + ');'
funTypedef.ignore(cStyleComment)
declaration = funTypedef.setResultsName('result') | 'DLL_PUBLIC' + function.setResultsName('result')


# will parse simple enum declarations (not yet used)
enumName = Word(alphas)
enumMember = Word(alphas)
enumDecl = enumKwd + enumName + '{' + Group(delimitedList(enumMember)) + '};'
enumDecl.ignore(cStyleComment)
headerSource= declaration | enumDecl
#headerSource.ignore(cStyleComment)

def makeBindingsForFile(source_file, gen_label):
    codeGenerator = CodeGenerators[gen_label]
    code = []
    try:
        with open(source_file, 'r') as file:
            source = file.read()
            for t in headerSource.searchString(source):
                code.append(t.asList())

    except ParseException as error:
        print(error)

    print '\n'.join(codeGenerator.emit(code))

def makeBindings():
    parser = argparse.ArgumentParser('creates bindings from a \'C\' header')
    parser.add_argument('--f', metavar = 'source_file', help = 'The \'C\' file to parse')
    parser.add_argument('--g', default='csharp', metavar = 'code_gen', help = 'The code generator to use, e.g \'python\'')
    args = parser.parse_args()

    if args.f is None:
        parser.print_usage()
        return
    elif args.g is None:
        parser.print_usage()
    elif not CodeGenerators.has_key(args.g):
        print 'Invalid code generator!. Valid Generators are:'
        for g in CodeGenerators: print `g`
    else:
        makeBindingsForFile(args.f, args.g)

if __name__ == "__main__":
    makeBindings()
