#ifndef __CPPGEN_GNET_SKILL314
#define __CPPGEN_GNET_SKILL314
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill314:public Skill
    {
      public:
        enum
        { SKILL_ID = 314 };
          Skill314 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill314Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHp () * 1000 / skill->GetPlayer ()->GetMaxhp ());
                float decgyghp = skill->GetPlayer ()->GetMaxhp () * (0.87 - 0.03 * skill->GetLevel () - 0.01 * skill->GetLevel () * skill->GetT2 ());
                  skill->GetPlayer ()->SetHp (norm ((skill->GetPlayer ()->GetHp () - 0.5) / decgyghp) * (skill->GetPlayer ()->GetHp () - decgyghp) +
                                              1);
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill314Stub ():SkillStub (314)
        {
            occupation = 3;
            name = L"³ÕÇéÖä";
            nativename = "³ÕÇéÖä";
            icon = "³ÕÇéÖä.dds";
            maxlevel = 11;
            maxlearn = 8;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 647;
            talent[1] = 650;
            talent[2] = 660;
            talent[3] = 662;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[1] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[2] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[3] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[4] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[5] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 0;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill314Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 263 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (8 + 2 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (96 + 12 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 12000 * skill->GetT3 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 80, 82, 84, 86, 88, 90, 92, 94 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7 + 2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5 + 2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               96 + 12 * skill->GetLevel (),
                               87 - 3 * skill->GetLevel (),
                               14 + skill->GetLevel (),
                               15 + 2 * skill->GetLevel (), skill->GetLevel (), 87 - 3 * skill->GetLevel (), 5 + 2 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (14000 + 1000 * skill->GetLevel () + 100 + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (0.15 + 0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount ((skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ()) * (1 + 7.5 * skill->GetT1 ()));
            skill->GetVictim ()->SetDechurt (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (60000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.00001 + 0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetValue (0.06 * skill->GetT0 () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetAddhp2 (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * norm (skill->GetT1 ())));
            skill->GetVictim ()->SetTime (14000 + 1000 * skill->GetLevel () + 100 + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (80 * skill->GetLevel () * skill->GetT1 ());
            skill->GetVictim ()->SetAdddefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 950 ? 120 * norm (skill->GetT0 () / 3.9) : 0));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
