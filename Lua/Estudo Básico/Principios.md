# LUA

### Operadores Relacionais

**Igual A:**
     
     ==

**Maior que:**
     
     >
     
**Menor que:**
     
     <
     
**Maior ou igual A:**
     
     >=
     
**Menor ou igual A:**
     
     <=
     
**Diferente de:**
     
     ~=
     
### Operadores Lógicos

**conjunção (.E.):**
     
     and
     
**Disjunção (.OU.):**
     
     or
     
**Negação (.NÂO.):**
     
     not
     
 ### Tipos de decisão
 
 **decisão com unico fluxo**
      
      if (condição) then
        ação
      end

**decisão com fluxo duplo**
     
     if(condição) then
        ação
     else
        ação
     end
     
### Tipos de laços

**Pre-teste**
     
     while (condição) do
       ação
     end

**Pos-teste**
     
     repeat
       ação
     until (condição)
     
**interativo**
     
     for (inicio, fim, passo)
       ação
     end
     
 ### Argumento Arbitrário
      
      function FATORIAL (...)
        local I, FAT
        FAT = 1
        for I = 1, ... do
          FAT = FAT * I
        end
        return FAT
     end
     
> Os três pontos representam uma expressão na liguagem lua sendo parecido a uma variavel
      
