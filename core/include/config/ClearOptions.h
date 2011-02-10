#ifndef CORE_CONFIG_CLEAROPTIONS_H_
#define CORE_CONFIG_CLEAROPTIONS_H_

#include <set>
#include <vector>
#include <string>
#include <iterator>

#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

class ClearOptions {
public:
	ClearOptions();

	virtual ~ClearOptions();

    shared_ptr<ClearOptions> clone() const;

    string describe() const;
    const set<string>& preservedSchemas() const;
	const set<string>& preservedTables() const;
	const set<string>& preservedViews() const;
	const set<string>& preservedFunctions() const;
	const set<string>& preservedMaterializedViews() const;
	const set<string>& preservedSynonyms() const;
	const set<string>& preservedSequences() const;
	const set<string>& preservedDataOnlySchemas() const;
	const set<string>& preservedDataOnlyTables() const;


    void preservedSchemas(const string& schemas);
    void preservedTables(const string& tables);
    void preservedViews(const string& views);
    void preservedFunctions(const string& functions);
    void preservedMaterializedViews(const string& materializedViews);
    void preservedSynonyms(const string& synonyms);
    void preservedSequences(const string& sequences);
    void preservedDataOnlySchemas(const string& schemas);
    void preservedDataOnlyTables(const string& tables);

	void preservedSchemas(const vector<string>& schemas);
	void preservedTables(const vector<string>& tables);
	void preservedViews(const vector<string>& views);
	void preservedFunctions(const vector<string>& functions);
	void preservedMaterializedViews(const vector<string>& materializedViews);
	void preservedSynonyms(const vector<string>& synonyms);
	void preservedSequences(const vector<string>& sequences);
	void preservedDataOnlySchemas(const vector<string>& schemas);
	void preservedDataOnlyTables(const vector<string>& tables);

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

    template <class InputIterator>
    void preservedDataOnlySchemas(InputIterator first, InputIterator last){
        m_preservedDataOnlySchemas.insert(first, last);
    }

    template <class InputIterator>
    void preservedDataOnlyTables(InputIterator first, InputIterator last){
        m_preservedDataOnlyTables.insert(first, last);
    }

	void preservedSchema(const string& schema);
	void preservedTable(const string& table);
	void preservedView(const string& view);
	void preservedFunction(const string& function);
	void preservedMaterializedView(const string& materializedView);
	void preservedSynonym(const string& synonym);
	void preservedSequence(const string& sequence);
	void preservedDataOnlySchema(const string& schema);
	void preservedDataOnlyTable(const string& table);

	bool isPreservedSchema(const string& schema);
	bool isPreservedTable(const string& table);

private:
	const string describe(string name, const set<string>& options) const;

	set<string> m_preservedSchemas;
	set<string> m_preservedTables;
	set<string> m_preservedViews;
	set<string> m_preservedFunctions;
	set<string> m_preservedMaterializedViews;
	set<string> m_preservedSynonyms;
	set<string> m_preservedSequences;
	set<string> m_preservedDataOnlySchemas;
	set<string> m_preservedDataOnlyTables;
};

#endif /* CORE_CONFIG_CLEAROPTIONS_H_ */
