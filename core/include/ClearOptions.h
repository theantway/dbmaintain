#ifndef CLEAROPTIONS_H_
#define CLEAROPTIONS_H_

#include <set>
#include <string>
#include <iterator>
using namespace std;

class ClearOptions {
public:
	ClearOptions();
	ClearOptions(const ClearOptions& options);

	virtual ~ClearOptions();

	const set<string> preservedSchemas() const;
	const set<string> preservedTables() const;
	const set<string> preservedViews() const;
	const set<string> preservedFunctions() const;
	const set<string> preservedMaterializedViews() const;
	const set<string> preservedSynonyms() const;
	const set<string> preservedSequences() const;

	void preservedSchemas(const set<string> schemas);
	void preservedTables(const set<string> tables);
	void preservedViews(const set<string> views);
	void preservedFunctions(const set<string> functions);
	void preservedMaterializedViews(const set<string> materializedViews);
	void preservedSynonyms(const set<string> synonyms);
	void preservedSequences(const set<string> sequences);

	template <class _InputIterator>
	void preservedSchemas(_InputIterator first, _InputIterator last){
		m_preservedSchemas.insert(first, last);
	}

	template <class InputIterator>
	void preservedTables(InputIterator first, InputIterator last){
		m_preservedTables.insert(first, last);
	}

	template <class InputIterator>
	void preservedViews(InputIterator first, InputIterator last){
		m_preservedViews.insert(first, last);
	}

	template <class _InputIterator>
	void preservedFunctions(_InputIterator first, _InputIterator last){
		m_preservedFunctions.insert(first, last);
	}

	template <class InputIterator>
	void preservedMaterializedViews(InputIterator first, InputIterator last){
		m_preservedMaterializedViews.insert(first, last);
	}

	template <class InputIterator>
	void preservedSynonyms(InputIterator first, InputIterator last){
		m_preservedSynonyms.insert(first, last);
	}

	template <class InputIterator>
	void preservedSequences(InputIterator first, InputIterator last){
		m_preservedSequences.insert(first, last);
	}

	void preservedSchema(string schema);
	void preservedTable(string table);
	void preservedView(string view);
	void preservedFunction(string function);
	void preservedMaterializedView(string materializedView);
	void preservedSynonym(string synonym);
	void preservedSequence(string sequence);

	bool isPreservedTable(string table);

private:
	set<string> m_preservedSchemas;
	set<string> m_preservedTables;
	set<string> m_preservedViews;
	set<string> m_preservedFunctions;
	set<string> m_preservedMaterializedViews;
	set<string> m_preservedSynonyms;
	set<string> m_preservedSequences;
};

#endif /* CLEAROPTIONS_H_ */
