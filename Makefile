CXXFLAGS=-pthread -D_POSIX_C_SOURCE=200809L -g -O0
# Só ativa sanitizers se não estivermos no cygwin nem num vgbuild
ifneq ($(OS),Windows_NT)
	ifneq ($(DISABLE_SANS),1)
#		CXXFLAGS +=  -fsanitize=address -fsanitize=undefined
	endif
endif
LFLAGS=
OUTPUT=program
LIBS=-lm

# Essas duas coisas servem pra mapear quais arquivos .cpp dependem de 
# quais arquivos .h. Lembre-se que se o .h muda, todo .cpp que inclui 
# ele precisa ser compilado novamente. Esse makefile usa features 
# do GCC pra que o próprio GCC dê essa informação (seria chato 
# listar essas relações aqui no Makefile)
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
DEPFLAGS=-MT $@ -MMD -MP -MF build/$*.Td

# Escaneia todos os arquivos .cpp na raiz do projeto
SOURCES=$(wildcard *.cpp)
# Transforma a lista de arquivos .cpp numa lista de arquivos .o em build/
# O := faz com que o lado direito seja expando AGORA, e não mais tarde
OBJS:=$(patsubst %.cpp,build/%.o,$(SOURCES))

# all, submission e clean sempre rodam (sem checar se suas dependencias 
# estão sujas ou não)
.PHONY: all submission clean

# Cria pastas internas, o usuário querendo ou não
$(shell mkdir -p $(DEPDIR) build >/dev/null)

# Esse é o primeiro alvo não especial. make == make all 
# e make all == make program
all: build/program

# Compila o programa para uso com valgrind (desativa sanitizers)
# Também gera um script run-valgrind.sh
vgbuild: clean
	$(MAKE) DISABLE_SANS=1 all
	echo -e "#!/bin/bash\nvalgrind --leak-check=full ./program \"\$$@\"" > run-valgrind.sh
	chmod +x run-valgrind.sh

# Apaga essa regra, se existir
%.o: %.cpp
# Todo arquivo .o tem um .cpp como dependencia. Quando o .cpp muda, o .o precisa 
# ser recompilado. $@ expande pro arquivo .o em build/ e $< expande pro 
# arquivo .cpp correspondente
# O arquivo .d guarda info de dependência de headers, se ele muda, também 
# precisamos recompilar.
build/%.o : %.cpp build/%.d
	$(CXX) -Wall -Werror -std=c++11 $(CXXFLAGS) $(DEPFLAGS) -o $@ -c $<
	mv -f build/$*.Td build/$*.d && touch $@

# Junta todos os arquivos .o em um único magnífico binário
# Se qualquer .o em $(OBJ) mudar, program precisa ser reconstruído
# O Make não entende wildcards na lista de dependências, por isso 
# a variável OBJ foi computada anteriormente, com :=
# $^ significa *todas* as dependências ($< pega só a primeira)
build/program: $(OBJS)
	$(CXX) -Wall -Werror -std=c++11 $(CXXFLAGS) $(LFLAGS) -o $@ $^ $(LIBS)
	cp build/program $(OUTPUT)

# Prepara .tar.gz pra submissão no moodle
# Note que antes de preparar o tar.gz, é feito um clean
submission: all
	rm -fr build program
	@echo -e "\nEsse é um Makefile sem grade.sh associado. Não irei atribuir nota!"
	@rm -fr build $(OUTPUT)
	@SUBNAME=$$(basename "$$(pwd)"); \
		echo cd ..\; tar zcf "$$SUBNAME.tar.gz" "$$SUBNAME"
	@SUBNAME=$$(basename "$$(pwd)"); \
		cd ..; \
		rm -fr "$$SUBNAME.tar.gz"; \
		tar zcf "$$SUBNAME.tar.gz" "$$SUBNAME"
	@echo "Criado pacote \"$$(pwd).tar.gz\" para submissão"

# Limpa binários
clean:
	rm -fr build $(OUTPUT)

# Só faltam alguns  truques pra dependências automáticas de headers

# Todo arquivo de dependência é também um target. (evita erro no primeiro build)
build/%.d: ;
# Pede pro make não tratar como arquivo intermediário
.PRECIOUS: build/%.d 

# Transforma a lista de arquivos .cpp em uma lista de arquivos .d em build/
# Inclui esses arquivos .d (que contêm dependências de headers) como se 
# fossem parte desse makefile
include $(wildcard $(patsubst %,build/%.d,$(basename $(SOURCES))))
