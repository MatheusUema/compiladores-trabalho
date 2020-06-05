int main()
{
    char token[30];
}

programa()
{
    if (token == 'program')
    {
        token = obter_simbolo();
        if (token == 'ident')
        {
            token = obter_simbolo();
            if (token == ';')
            {
                dc_c();
                dc_v();
                dc_p();
                if (token == 'begin')
                {
                    comandos();
                    if (token == 'end')
                    {
                        token = obter_simbolo();
                        if (token == '.')
                        {
                            return SUCESSO;
                        }
                        else
                            return ERRO;
                    }
                    else
                    {
                        return ERRO;
                    }
                }
                else
                    return ERRO;
            }
            else
                return ERRO;
        }
        else
            return ERRO;
    }
    else
        return ERRO;
}

dc_c()
{
    if (token != 'const')
    {

        sair();
    }
    else // Presumi aqui que se um simbolo não é diferente de var, ele é igual
    {
        token = obter_simbolo();
        if (token == 'ident')
        {
            token = obter_simbolo();
            if (token == '=')
            {
                if ((token == 'numero_int') || (token == 'numero_real'))
                {
                    token = obter_simbolo();
                    if (token == ';')
                    {
                        token = obter_simbolo();
                        if ((termino_cadeia == falso) && (token == "const"))
                        {
                            dc_c();
                        }
                    }
                    else
                    {
                        return ERRO;
                    }
                }
                else
                {
                    return ERRO;
                }
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
}

dc_v()
{
    if (token != 'var')
    {

        sair();
    }
    else // Presumi aqui que se um simbolo não é diferente de var, ele é igual
    {
        token = obter_simbolo();
        variaveis();
        if (token == ':')
        {
            token = obter_simbolo();
            if ((token == 'real') || (token == 'integer'))
            {
                token = obter_simbolo();
                if (token == ';')
                {
                    token = obter_simbolo();
                    if ((termino_cadeia == falso) && (token == "var"))
                    {
                        dc_v();
                    }
                }
                else
                {
                    return ERRO;
                }
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
}

variaveis()
{
    if (token == 'ident')
    {
        token = obter_simbolo();
        if (token != ',')
        {
            sair();
        }
        else // Presumi aqui que se um simbolo não é diferente de var, ele é igual
        {
            token = obter_simbolo();
            if ((termino_cadeia == falso) && (token == "ident"))
            {
                variaveis();
            }
        }
    }
    else
    {
        return ERRO;
    }
}

dc_p()
{
    if (token != 'procedure')
    {
        sair();
    }
    else // Presumi aqui que se um simbolo não é diferente de var, ele é igual
    {
        token = obter_simbolo();
        if (token == 'ident')
        {
            token = obter_simbolo();
            parametros();
            if (token == ';')
            {
                token = obter_simbolo();
                corpo_p();
                if ((termino_cadeia == falso) && (token == "procedure"))
                {
                    dc_p();
                }
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
}

parametros()
{
    if (token != '(')
    {
        sair();
    }
    else // Presumi aqui que se um simbolo não é diferente de var, ele é igual
    {
        token = obter_simbolo();
        variaveis();
        if (token == ':')
        {
            token = obter_simbolo();
            if ((token == 'real') || (token == 'integer'))
            {
                token = obter_simbolo();
                while (token == ';')
                {
                    token = obter_simbolo();
                    variaveis();
                    if (token == ':')
                    {
                        token = obter_simbolo();
                        if ((token == 'real') || (token == 'integer'))
                            token = obter_simbolo;
                    }
                    else
                    {
                        return ERRO;
                    }
                }
                if (token = -')')
                {
                    token = obter_simbolo;
                    return SUCESSO;
                }
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
}

corpo_p()
{
    dc_v();
    if (token == 'begin')
    {
        token = obter_simbolo();
        comandos();
        if (token == 'end')
        {
            token = obter_simbolo();
            if (token == ';')
            {
                token = obter_simbolo();
                return SUCESSO;
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
    else
    {
        return ERRO;
    }
}

comandos() // Esse aqui acho que tá errado,não consegui pensar num jeito em que cmd não precise ser executado
{
    cmd();
    while (token == ';')
    {
        cmd;
    }
    sair();
}


cmd() // Pensei em usar um switch case, mas não serve p/ string, to pensando em como retornar um erro caso não seja nenhuma das opções, colocar um else no ultimo if funciona?
{
    if (token == 'read')
    {
        token = obter_simbolo;
        if (token == '(')
        {
            token = obter_simbolo;
            variaveis();
            if (token == ')')
            {
                return SUCESSO;
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
    if (token == 'write')
    {
        token = obter_simbolo;
        if (token == '(')
        {
            token = obter_simbolo;
            variaveis();
            if (token == ')')
            {
                token = obter_simbolo;
                return SUCESSO;
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
    if (token == 'while')
    {
        token = obter_simbolo;
        if (token == '(')
        {
            token = obter_simbolo;
            variaveis();
            if (token == ')')
            {
                token = obter_simbolo;
                if (token == 'do')
                {
                    token = obter_simbolo;
                    cmd();
                }
                else
                {
                    return ERRO;
                }
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
    if (token == 'if')
    {
        token = obter_simbolo;
        condicao();
        if (token = 'then')
        {
            token = obter_simbolo;
            cmd();
            if (token = 'else')
            {
                token = obter_simbolo;
                cmd();
            }
            else
            {
                sair();
            }
        }
        else
        {
            return ERRO;
        }
    }
    if (token == 'ident')
    {
        token = obter_simbolo;
        if (token == ':=')
        {
            token = obter_simbolo();
            expressao();
        }
        else
        {
            lista_arg();
        }
    }
    if (token == 'begin')
    {
        token = obter_simbolo();
        comandos();
        if (token == 'end')
        {
            token = obter_simbolo();
            return SUCESSO;
        }
        else
        {
            return ERRO;
        }
    }
    if (token == 'for')
    {
        token = obter_simbolo();
        if (token == 'ident')
        {
            token = obter_simbolo();
            if (token == ':=')
            {
                token = obter_simbolo();
                if (token == 'numero_int')
                {
                    token = obter_simbolo();
                    if (token == 'to')
                    {
                        token = obter_simbolo();
                        if (token == 'numero_int')
                        {
                            token = obter_simbolo();
                            if (token == 'do')
                            {
                                token = obter_simbolo();
                                cmd();
                            }
                            else
                            {
                                return ERRO;
                            }
                        }
                        else
                        {
                            return ERRO;
                        }
                    }
                    else
                    {
                        return ERRO;
                    }
                }
                else
                {
                    return ERRO;
                }
            }
            else
            {
                return ERRO;
            }
        }
        else
        {
            return ERRO;
        }
    }
}

condicao()
{
    expressao();
    if ((token == '=') || (token == '<>') || (token == '>=') || (token == '<=') || (token == '>') || (token == '<'))
    {
        token = obter_simbolo();
        expressao();
    }
    else
    {
        return ERRO;
    }
}

fator()
{
    if ((token == 'ident') || (token == 'numero_int') || (token == 'numero_real'))
    {
        token = obter_simbolo();
        sair();
    }
    else
    {
        if (token == '(')
        {
            token = obter_simbolo();
            expressao();
            if (token == ')')
            {
                token = obter_simbolo();
                return SUCESSO;
            }
        }
        else
        {
            return ERRO;
        }
    }
}

expressao() // Tenho dúvidas se essa função está correta
{
    termo();
    while ((token == '+') || (token == '-'))
    {
        termo();
    }
}

termo()
{ // Acho que esse tá um pouco errado pq não to garantinho a inseração de termos sem + ou -
    if ((token == '+') || (token == -))
    {
        token = obter_simbolo();
        fator();
        while ((token == '+') || (token == '-'))
        {
            fator();
        }
    }
}

lista_arg()
{
    if (token == '(')
    {
        token = obter_simbolo();
        if (token == 'ident')
        {
            token = obter_simbolo();
            while (token == ';')
            {
                token = obter_simbolo();
                if (token == 'ident')
                {
                    token = obter_simbolo();
                }
                else
                {
                    return ERRO;
                }
            }
            if (token == ')')
            {
                return SUCESSO;
            }
        }
        else
        {
            return ERRO;
        }
    }
    else
    {
        return ERRO;
    }
}