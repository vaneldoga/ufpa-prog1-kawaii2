require 'prawn'
require 'prawn/table'
require 'prawn/measurement_extensions'

class SBCDocument < Prawn::Document
	def initialize options = {}, &block
		@counters = { sections: 0, tables: 0 }
		@font_size = 12
		@authors = options.fetch :authors
		@institution = options.fetch :institution, ["Universidade de XXXXX", "Instituto de XXXXX"]
		@local = options.fetch :local, "City - State"
		@year = options.fetch :year, "20xx"
		@title = options.fetch :title, "Title"
		@abstract = options.fetch :abstract, "blablablablabla."
		super(margin: 3.cm)
		self.font_families.update("DejaVuSerif" => {
			normal: '/usr/share/fonts/TTF/DejaVuSerif.ttf',
			italic: '/usr/share/fonts/TTF/DejaVuSerif-Italic.ttf',
			bold: '/usr/share/fonts/TTF/DejaVuSerif-Bold.ttf',
			bold_italic: '/usr/share/fonts/TTF/DejaVuSerif-BoldItalic.ttf'
		})
		self.font 'DejaVuSerif'
		if options.fetch(:front_matter, false)
			self.make_front_matter
		else
			self.make_header
		end
	end

	def make_header
		self.text @title, size: 14, style: :bold, align: :center
		self.move_down @font_size*1.5
		self.text @authors.map{|a| a[:name]+'¹'}.join(", "), size: @font_size, style: :bold, align: :center
		self.move_down @font_size*1.5
		self.text '¹'+@institution.join(" - "), size: @font_size, align: :center
		self.move_down @font_size*1.5
		self.text @authors.map{|a| a[:email]}.join(", "), size: @font_size, style: :italic, align: :center
		self.move_down @font_size*1.5
		self.indent(1.5.cm, 1.5.cm) do
			formatted_text(
				[
					{ text: "Abstract. ", size: @font_size, styles: [:bold, :italic] },
					{ text: @abstract, size: @font_size, styles: [:italic] }
				],
				align: :justify
			)
		end
		self.move_down @font_size*1.5
	end

	def make_front_matter
		self.text @institution.join("\n"), size: 14, style: :bold, align: :center, valign: :top
		self.formatted_text(
			[
				{ text: @title + "\n\n", size: 30, styles: [:bold] },
				{ text: @authors.join("\n") }
			],
			align: :center,
			valign: :center
		)
		self.text "#{@local}\n#{@year}", align: :center, valign: :bottom
		self.start_new_page
	end

	def add_header text
		@counters[:sections] += 1
		self.text "#{@counters[:sections]} #{text}", style: :bold, size: @font_size*1.5
		self.move_down @font_size*1.5
	end

	def add_text text
		self.text text, align: :justify
		self.move_down @font_size*1.5
	end

	def add_table table
		@counters[:tables] += 1
		width = self.bounds.width * 0.9
		# Title, blank space + 1st row + margin
		minimum_height = @font_size + @font_size*1.5 + @font_size + self.page.margins[:bottom]
		if cursor < minimum_height then start_new_page end
		self.text "Tabela #{@counters[:tables]}:", style: :bold, size: @font_size, align: :center
		self.move_down @font_size*1.5
		self.table(table) do
			row(0).font_style = :bold
			row(0).background_color = 'EEEEEE'
			self.header = true
			self.row_colors = ['FFFFFF', 'F9F9F9']
			self.width = width
			self.position = :center
		end
		self.move_down @font_size*1.5 
	end
end

pdf = SBCDocument.new({
	institution: [
		"Universidade Federal do Pará",
		"Instituto de Ciências Exatas e Naturais",
		"Programação I"
	],
	authors: [
		{name: "Mateus Cezário Barreto", email: "mateus.cezario.barreto@gmail.com"},
		{name: "Nicolas Alho"},
		{name: "Yuri Gabriel Delgado"}
	],
	local: "Belém - Pará",
	year: "2026",
	title: "CLI Pokedex",
	abstract: "blablablablab lalbalbalb alblablblba lblablblbla balbalbal balbalbalb albal, balblablablablallablablalbalbb, blbalblablal"
})

pdf.add_header "Introdução"

pdf.add_text "CLI Pokedéx é uma implementação de lista encadeada [Morin 2013] em C++, que foi desenvolvida como projeto final da disciplina de Programação I, ministrada pela Profa. Dra. Paula Cardoso. O projeto foi contextualizado na temática de Pokemons, onde cada bloco encadeado contém o registro de um pokemon, e a lista em si é chamada de Pokedex."

pdf.add_text "As requisições originais propostas ao projeto incluem o armazenamento e a leitura de estado a partir de um arquivo textual que também fosse manualmente editável. Essa tarefa seria substancialmente menos trabalhosa usando uma biblioteca de persistência de dados em formato textual, como o formato Json, e a própria biblioteca padrão do C++ (Standard Template Library ou STL) para obter a estrutura std::list, que implementa uma estrutura encadeada [Brokken 2014]. Nesse caso, o esforço principal seria em construir uma interface de linha de comando ou gráfica para que o usuário pudesse efetuar operações de criação, leitura, atualização e remoção dentro da aplicação e igualmente requeridas, conjunto esse de operações conhecido como CRUD (Create, Retrieve, Update and Delete) [Bourke 2019], bem como, adicionalmente, a operação de ordenamento baseada em, pelo menos, dois parâmetros."

pdf.add_text "No entanto, uma das exigências é justamente o não uso de estruturas como std::list (para lista encadeada) e std::vector (para sequência baseada em cache) [Brokken 2014], no intuito de que essa tarefa fosse reimplementada no código, com a conveniente flexibilidação para que os elementos da lista sejam apenas adicionados no final. No espírito didático dessa restrição didática, também escolheu-se implementar uma sintaxe básica para leitura e gravação de dados em arquivos, ao invés de usar uma biblioteca, o que só seria viável com um tipo de dado suficientemente simples, sem subtipos ou recursões."

pdf.add_text "Portanto, a simplicidade da estrutura de atributos foi um dos motivos para a escolha de Pokemons como temática do projeto. Cada registro de Pokemon inclui apenas tipos de texto e tipos numéricos, sendo o registro de um pokemon a única estrutura encadeada, isto é, tirando a necessidade de uma implementação genétrica de lista para lidar com vários tipos de dados."

pdf.add_text "No que tange a escolha de soluções e de ambiente, o projeto foi ambientado no ecossistema de sistemas operacionais baseados em Linux, com uso de ferramentas de código aberto e de material teórico de licença permissiva, sempre que possível."

pdf.add_header "Planejamento"

pdf.add_text "O projeto da CLI Pokedex foi desenvolvido usando o sistema de versionamento Git, e publicado no serviço de hospedagem de código Github desde o começo do desenvolvimento. O planejamento foi relaizado dentro do próprio repositório Git, nos arquivos \"readme.md\" (língua inglesa) e \"readme.pt-BR.md\" (língua portuguesa), iniciando com a escolha dos campos para serem utilizados como atribuitos de cada pokemon que fosse registrado pelo programa. Os atribuitos escolhidos abrangeram tipos numéricos inteiros e decimais, bem como tipos textuais."

pdf.add_table [
	["Nome do atributo", "Tradução", "Tipo"],
	["name", "nome", "string"],
	["global_id", "ID global", "int"],
	["weight", "peso", "float"],
	["generation", "geração", "int"]
]

pdf.add_text "Para alguns atrtibutos tipificados em containeres, como as habilidades, forças e fraquezas de cada pokemon, utilizou-se reservade espaço sequencial (array), já que a janela de variação do número desses atribuitos não é tão grande. Nesse sentido, a reserva de espaço para cada sequência contínua desses atributos foi de 5, o suficiente para comportar as variações de número de elementos sem implicar em um grande uso de espaço em memória que justificasse outro tipo de lista encadeada."

pdf.add_text "Nos primeiros commits, é possível perceber que a biblioteca std::vector estava sendo usada. A inclusão da biblioteca std::vector nas etapas iniciais do código foi feita de forma temporária, para focar no desenvolvimento da interface de linha de comando, antes de que o std::vector desse lugar à implementação própria de lista encadeada dentro da classe Pokedex."

pdf.add_text "As versões iniciais também incluiam uso de structs e classes, misturadas. Mais especificamente, as structs foram usadas para o registro dos atribuitos do pokemon e para um elemento da lista encadeada (reúne o registro dos atributos mais dois campos de ponteiro, contendo o endereço do elemento sucessor e do antecessor), enquanto a pokedex em si foi construída em forma de classe, para reunir métodos de manipulação da lista encadeada. No entanto, o uso de structs foi abandonado em detrimento do uso de classes, apenas, para adotar uma idioma mais próximo de orientação à objetos."

pdf.add_text "Por outro lado, uma versão paralela da CLI Pokedex, escrita em linguagem C e sem orientação à objetos, foi planejada, mas não chegou a ser implementada. Uma interface gráfica para a Pokedex, com o uso da biblioteca de interface de modo imediato Raygui, foi outra funcionalidade planejada que não chegou a ser escrita, apesar de arquivos relacionados à essa interface alternativa estarem presentes no código fonte."

pdf.add_text "Já o sistema de construção escolhido para o programa foi o Meson, pela proximidade da sintaxe dos arquivos de construção do Meson com a linguagem C++, pelo menos no que tange à listas e chamada de função. Ainda assim, visando melhor portabilidade e tendo em vista o pequeno porte do projeto, o repositório contém instruções para compilação manual, sem o Meson, apenas com o uso de um compilador de C++, como gcc (comando g++) ou clang (comando clang++)."

pdf.add_text "Em relação à padronização, o padrão ISO da linguagem C++ utilizado foi o de 2023 (C++23), conquanto os recursos da linguagem utilizados se ateram ao padrão, com exceção da estrutura \"enum class\". Mesmo a entrada e saída não utilizou os modelos recentes de formatação com print, e sim os tradicionais operadores de stream (\">>\" e \"<<\"), junto com funções oriundas da linguagem C, principalmente para ocasiões com menos formatação, como as mensagens de error. O processamento de argumentos da interface de comando foi outra ocasião em que funções oriundas de C foram preferidas, em razão da velocidade."

pdf.add_text "Por fim, o próprio relatório da atividade avaliativa foi escrito dentro do repositório Git, em formato Markdown, para ser montável assim como o resto do programa. Em paralelo à escrita do relatório, utilizou-se uma outra linguagem de programação, Ruby, para gerar o documento PDF, em um visual próximo ao padrão da Sociedade Brasileira de Computação."

pdf.add_header "Resultados"

pdf.add_text "O programa é capaz de performar operações básicas de CRUD com o uso de parâmetros imediatos passados via linha de comando, exceto pela operação de atualizar, o que só pode ser feito manualmente, editando o arquivo de texto."

pdf.add_header "Referências"

pdf.add_text "Morin, P. (2013). Open Data Structures. AU Press."

pdf.add_text "Bourke, C. (2019). Computer Science II. https://cse.unl.edu/~cbourke/ComputerScienceTwo.pdf."

pdf.add_text "Brokken, F. B. (2014). C++ Annotations. ISBN 90-367-0470-7. University of Groningen."

pdf.render_file "build/relatorio.pdf"

